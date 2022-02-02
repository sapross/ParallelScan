#pragma once

namespace openmp
{
namespace updown
{
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<typename InputIter, typename OutputIter, typename BinaryOperation>
OutputIter inclusive_scan(InputIter       first,
                          InputIter       last,
                          OutputIter      d_first,
                          BinaryOperation binary_op)
{
    size_t num_values = last - first;
    size_t step       = 2;
// Up sweep

// First stage of the up sweep fused with copy.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t left = i + step / 2 - 1, right = i + step - 1;
        d_first[left]  = first[left];
        d_first[right] = binary_op(first[left], first[right]);
    }

    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            d_first[right] = binary_op(d_first[left], d_first[right]);
        }
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));
    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
#pragma omp parallel for
        for (size_t i = step; i < (num_values - 1); i = i + step)
        {
            d_first[i + step / 2 - 1] =
                binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
        }
    }
    return d_first + num_values;
}

template<typename InputIter, typename OutputIter>
OutputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first)
{
    return openmp::updown::inclusive_scan(first, last, d_first, std::plus<>());
}

template<typename InputIter> InputIter inclusive_scan(InputIter first, InputIter last)
{
    size_t num_values = last - first;
    size_t step       = 1;

    // Up sweep
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {

        step = step * 2;

#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            first[right] = (first[left] + first[right]);
        }
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));

    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
#pragma omp parallel for
        for (size_t i = step; i < num_values - 1; i = i + step)
        {
            first[i + step / 2 - 1] = (first[i - 1] + first[i + step / 2 - 1]);
        }
    }
    return last;
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename OutputIter, typename T, typename BinaryOperation>
OutputIter exclusive_scan(InputIter       first,
                          InputIter       last,
                          OutputIter      d_first,
                          T               init,
                          BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<InputIter>::value_type;
    static_assert(std::is_same<ValueType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 2;
// Up sweep

// First stage of the up sweep fused with copy.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t left = i + step / 2 - 1, right = i + step - 1;
        d_first[left]  = first[left];
        d_first[right] = binary_op(first[left], first[right]);
    }
    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            d_first[right] = binary_op(d_first[left], d_first[right]);
        }
    }

    d_first[num_values - 1] = init;

    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t    left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            ValueType val_left = d_first[left], val_right = d_first[right];
            d_first[left]  = val_right;
            d_first[right] = binary_op(val_left, val_right);
        }
    }
    return d_first + num_values;
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter exclusive_scan(InputIter first, InputIter last, OutputIter d_first, T init)
{
    return openmp::updown::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<typename InputIter, typename T>
InputIter exclusive_scan(InputIter first, InputIter last, T init)
{
    using ValueType = typename std::iterator_traits<InputIter>::value_type;
    static_assert(std::is_same<ValueType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 2;

    // Up sweep

    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            first[right] = (first[left] + first[right]);
        }
    }

    first[num_values - 1] = init;
    // Down sweep

    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t    left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            ValueType val_left = first[left], val_right = first[right];
            first[left]  = val_right;
            first[right] = (val_left + val_right);
        }
    }

    return last;
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename BinaryOperation>
InputIter inclusive_segmented_scan(InputIter       first,
                                   InputIter       last,
                                   InputIter       d_first,
                                   BinaryOperation binary_op)
{
    using PairType = typename std::iterator_traits<InputIter>::value_type;
    using FlagType = typename std::tuple_element<1, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");

    return openmp::updown::inclusive_scan(first,
                                          last,
                                          d_first,
                                          [binary_op](PairType x, PairType y)
                                          {
                                              PairType result = y;
                                              if (!y.second)
                                              {
                                                  result.first =
                                                      binary_op(x.first, y.first);
                                                  // Since additions are reordered
                                                  // flags need to be carried along
                                                  // to indicate finished segments!
                                                  if (x.second)
                                                  {
                                                      result.second = x.second;
                                                  }
                                              }
                                              return result;
                                          });
}

template<typename InputIter, typename OutputIter>
OutputIter inclusive_segmented_scan(InputIter first, InputIter last, OutputIter d_first)
{
    return openmp::updown::inclusive_segmented_scan(first, last, d_first, std::plus<>());
}

template<typename InputIter>
InputIter inclusive_segmented_scan(InputIter first, InputIter last)
{
    size_t num_values = last - first;
    size_t step       = 1;

    // Up sweep

    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            if (not first[right].second)
            {
                first[right].first += first[left].first;
                if (first[left].second)
                {
                    first[right].second = first[left].second;
                }
            }
        }
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));

    // Down sweep

    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;

#pragma omp parallel for
        for (size_t i = step; i < num_values - 1; i += step)
        {
            size_t left = i - 1, right = i + step / 2 - 1;
            if (not first[right].second)
            {
                first[right].first += first[left].first;
                if (first[left].second)
                {
                    first[right].second = first[left].second;
                }
            }
        }
    }

    return last;
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename BinaryOperation, typename T>
InputIter exclusive_segmented_scan(InputIter       first,
                                   InputIter       last,
                                   InputIter       d_first,
                                   T               identity,
                                   T               init,
                                   BinaryOperation binary_op)
{
    /* Due to the add-swap operation in the down sweep phase a simple wrapper
       of the binary operation is insufficient.
     */
    using PairType  = typename std::iterator_traits<InputIter>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to First pair type!");

    size_t                num_values = last - first;
    size_t                step       = 1;
    std::vector<FlagType> temp_flags(num_values);
    /* Up sweep
       Essentially the same operation as with the conditional wrapper but
       now unpacked. This step is hence equal to the segmented inclusive
       up-down sweeping scan.
    */

    // First stage of the up sweep fused with copy
    step = step * 2;
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t   left = i + step / 2 - 1, right = i + step - 1;
        PairType val_left = first[left], val_right = first[right];

        // Copy flags into temp_flags.
        temp_flags[left] = val_left.second;
        // If left operand is segment start, mark right operand as finished.
        temp_flags[right] = val_left.second ? val_left.second : val_right.second;
        if (not val_right.second)
        {
            val_right.first = binary_op(val_left.first, val_right.first);
        }

        d_first[left]  = val_left;
        d_first[right] = val_right;
    }

    // Remainder stages of the up sweep.
    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            if (!temp_flags[right])
            {
                d_first[right].first =
                    binary_op(d_first[left].first, d_first[right].first);
                if (temp_flags[left])
                {
                    temp_flags[right] = temp_flags[left];
                }
            }
        }
    }

    d_first[num_values - 1].first = identity;

    // Down sweep
    for (int stage = std::floor(std::log2(num_values)) - 1; stage > 0; stage--)
    {
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            /*Add-Swap Operation needs to be handled differently than with all the
              other variants. Flags from the previous phase remain unmodified.
              There are two cases to be observed.
             */
            ValueType val_left = d_first[left].first, val_right = d_first[right].first;
            if (not temp_flags[left])
            {
                /*Left operand is not a segment beginning:
                  Independent of the right operand, swap the left value
                  with the right and save the sum in the right operand.

                  The segment beginnings will be reset to the init value at
                  a later stage and are currently required to hold intermediate
                  results.
                 */
                d_first[left].first  = val_right;
                d_first[right].first = binary_op(val_left, val_right);
            }
            else
            {
                /*Left operand is a segment beginning:
                  Independent of the right operand, swap the operand values.
                  This rule moves the already correctly calculated values into
                  their right place.
                 */
                d_first[left].first  = val_right;
                d_first[right].first = val_left;
            }
        }
    }

// Last stage of down-sweep meaning that stage = 0
// This stage is fused with a cleanup of the segment beginnings.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + 2)
    {
        size_t    left = i, right = i + 1;
        ValueType val_left = d_first[left].first, val_right = d_first[right].first;
        ValueType temp = val_left;

        // Modified rules to cause segment starts to be overwritten with init.
        if (not first[left].second and i != 0)
        {
            if (not first[right].second)
            {
                val_left  = val_right;
                val_right = binary_op(temp, val_right);
            }
            else
            {
                val_left  = d_first[right].first;
                val_right = identity;
            }
        }
        else
        {
            if (not first[right].second)
            {
                val_left  = identity;
                val_right = temp;
            }
            else
            {
                val_left  = identity;
                val_right = identity;
            }
        }
        d_first[left].first  = binary_op(init, val_left);
        d_first[right].first = binary_op(init, val_right);
    }
    return first + num_values;
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter exclusive_segmented_scan(
    InputIter first, InputIter last, OutputIter d_first, T identity, T init)
{
    return openmp::updown::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<typename InputIter, typename T>
InputIter exclusive_segmented_scan(InputIter first, InputIter last, T identity, T init)
{
    /* Due to the add-swap operation in the down sweep phase a simple wrapper
       of the binary operation is insufficient.
     */
    using PairType  = typename std::iterator_traits<InputIter>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to First pair type!");

    size_t                num_values = last - first;
    size_t                step       = 1;
    std::vector<FlagType> temp_flags(num_values);
    /* Up sweep
       Essentially the same operation as with the conditional wrapper but
       now unpacked. This step is hence equal to the segmented inclusive
       up-down sweeping scan.
    */

    // First stage of the up sweep fused with copy of the flags.
    step = step * 2;

#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t   left = i + step / 2 - 1, right = i + step - 1;
        PairType val_left = first[left], val_right = first[right];

        // Copy flags into temp_flags.
        temp_flags[left] = val_left.second;
        // If left operand is segment start, mark right operand as finished.
        temp_flags[right] = val_left.second ? val_left.second : val_right.second;
        if (not val_right.second)
        {
            val_right.first = (val_left.first + val_right.first);
        }

        first[right] = val_right;
    }

    // Remainder stages of the up sweep.
    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            if (!temp_flags[right])
            {
                first[right].first = (first[left].first + first[right].first);
                if (temp_flags[left])
                {
                    temp_flags[right] = temp_flags[left];
                }
            }
        }
    }

    first[num_values - 1].first = identity;

    // Down sweep
    for (int stage = std::floor(std::log2(num_values) - 1); stage > 0; stage--)
    {
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            /*Add-Swap Operation needs to be handled differently than with all the
              other variants. Flags from the previous phase remain unmodified.
              There are two cases to be observed.
             */
            size_t    left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            ValueType val_left = first[left].first, val_right = first[right].first;
            if (not temp_flags[left])
            {
                /*Left operand is not a segment beginning:
                  Independent of the right operand, swap the left value
                  with the right and save the sum in the right operand.

                  The segment beginnings will be reset to the init value at
                  a later stage and are currently required to hold intermediate
                  results.
                 */
                first[left].first  = val_right;
                first[right].first = (val_left + val_right);
            }
            else
            {
                /*Left operand is a segment beginning:
                  Independent of the right operand, swap the operand values.
                  This rule moves the already correctly calculated values into
                  their right place.
                 */
                first[left].first  = val_right;
                first[right].first = val_left;
            }
        }
    }

    // Last stage of down-sweep meaning that stage = 0
    // This stage is fused with a cleanup of the segment beginnings.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + 2)
    {
        //        left = i + (1 << 0) - 1, right = i + (1 << (0 + 1)) - 1;
        size_t    left = i, right = i + 1;
        ValueType val_left = first[left].first, val_right = first[right].first;
        ValueType temp = val_left;

        // Modified rules to cause segment starts to be overwritten with init.
        if (not first[left].second and i != 0)
        {
            if (not first[right].second)
            {
                val_left  = val_right;
                val_right = (temp + val_right);
            }
            else
            {
                val_left  = first[right].first;
                val_right = identity;
            }
        }
        else
        {
            if (not first[right].second)
            {
                val_left  = identity;
                val_right = temp;
            }
            else
            {
                val_left  = identity;
                val_right = identity;
            }
        }
        first[left].first  = (init + val_left);
        first[right].first = (init + val_right);
    }

    return last;
}

} // namespace updown
} // namespace openmp
