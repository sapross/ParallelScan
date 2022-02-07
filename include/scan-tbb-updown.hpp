#pragma once
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <vector>

namespace _tbb
{

namespace updown
{
// // ----------------------------------------------------------------------------------
// //  Inclusive Scan
// // ----------------------------------------------------------------------------------
template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt
inclusive_scan(InputIt first, InputIt last, OutputIt d_first, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    size_t num_values = last - first;
    size_t step       = 1;
    // Up sweep

    if ((last - first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        tbb::parallel_for(size_t(0), num_values, step,
                            [&](auto i)
                                {
                                    d_first[i + step - 1] =
                                        binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
                                });
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));
    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
        tbb::parallel_for(step, num_values -1, step,
                            [&](auto i)
                                {
                                    d_first[i + step / 2 - 1] =
                                        binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
                                });
    }

    return d_first + num_values;
}

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::updown::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return _tbb::updown::inclusive_scan(first, last, first, std::plus<>());
}

// // ----------------------------------------------------------------------------------
// //  Exclusive Scan
// // ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(
    InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 1;

    // Up sweep
    // if (std::distance(first, d_first) != 0)
    // {
    std::copy(first, last, d_first);
    // }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        tbb::parallel_for(size_t(0),
                          num_values,
                          step,
                          [&](auto i) {
                              d_first[i + step - 1] = binary_op(d_first[i + step / 2 - 1],
                                                                d_first[i + step - 1]);
                          });
    }
    d_first[num_values - 1] = init;

    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
        size_t downstep = (1 << (stage + 1));
        tbb::parallel_for(size_t(0),
                          size_t(num_values),
                          downstep,
                          [&](auto i)
                          {
                              OutputType t = d_first[i + (1 << stage) - 1];
                              d_first[i + (1 << stage) - 1] =
                                  d_first[i + (1 << (stage + 1)) - 1];
                              d_first[i + (1 << (stage + 1)) - 1] =
                                  binary_op(t, d_first[i + (1 << (stage + 1)) - 1]);
                          });
    }

    return d_first + num_values;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return _tbb::updown::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return _tbb::updown::exclusive_scan(first, last, first, init, std::plus<>());
}

// // ----------------------------------------------------------------------------------
// //  Inclusive Segmented Scan
// // ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt inclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  OutputIt        d_first,
                                  BinaryOperation binary_op)
{
    using PairType   = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::tuple_element<1, PairType>::type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<PairType, OutputType>::value,
                  "Input type must be convertible to output type!");

    /*Sequential inclusive scan becomes the segmented variant by wrapping the
      passed binary_op into a new conditional binary operation. Assuming
      a addition with the running sum as operand x and the current value as
      operand y, resetting the sum to the value of y yields the correct result.
     */
    size_t num_values = last - first;
    _tbb::updown::inclusive_scan(first,
                                 last,
                                 d_first,
                                 [binary_op](PairType x, PairType y)
                                 {
                                     PairType result = y;
                                     if (!y.second)
                                     {
                                         result.first = binary_op(x.first, y.first);
                                         /* Only required if additions are
                                            reordered!
                                         */
                                         if (x.second)
                                         {
                                             result.second = x.second;
                                         }
                                     }
                                     return result;
                                 });

    return d_first + num_values;
}

template<class InputIt, class OutputIt>
OutputIt inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::updown::inclusive_segmented_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_segmented_scan(InputIt first, InputIt last)
{
    return _tbb::updown::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// // ----------------------------------------------------------------------------------
// //  Exclusive Segmented Scan
// // ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class BinaryOperation, class T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, OutputIt d_first, T identity, T init, BinaryOperation binary_op)
{
    /* Due to the add-swap operation in the down sweep phase a simple wrapper
       of the binary operation is insufficient.
     */
    using PairType  = typename std::iterator_traits<InputIt>::value_type;
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
    tbb::parallel_for(size_t(0), num_values, step,
                            [&](auto i)
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
                                });
    // Remainder stages of the up sweep.
    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        tbb::parallel_for(size_t(0), num_values, step,
                            [&](auto i)
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
                            });
    }

    d_first[num_values - 1].first = identity;

    // Down sweep
    for (int stage = std::floor(std::log2(num_values)) - 1; stage > 0; stage--)
    {
        step = (1 << (stage + 1));
        tbb::parallel_for(size_t(0), num_values, step,
                            [&](auto i)
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
                            });
    }

// Last stage of down-sweep meaning that stage = 0
// This stage is fused with a cleanup of the segment beginnings.
    step = 2;
    tbb::parallel_for(size_t(0), num_values, step,
                            [&](auto i)
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
                            });
    return first + num_values;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first,T identity, T init)
{
    return _tbb::updown::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, T identity, T init)
{
    return _tbb::updown::exclusive_segmented_scan(
        first, last, first, identity, init, std::plus<>());
}

}; // namespace updown
}; // namespace _tbb
