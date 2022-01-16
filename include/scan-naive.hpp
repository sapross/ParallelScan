#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>

namespace sequential
{
namespace naive
{

// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class IterType, class BinaryOperation>
IterType
inclusive_scan(IterType first, IterType last, IterType d_first, BinaryOperation binary_op)
{
    return std::inclusive_scan(first, last, d_first, binary_op);
}

template<class IterType>
IterType inclusive_scan(IterType first, IterType last, IterType d_first)
{
    return sequential::naive::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_scan(IterType first, IterType last)
{
    return sequential::naive::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class IterType, class T, class BinaryOperation>
IterType exclusive_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    return std::exclusive_scan(first, last, d_first, init, binary_op);
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, IterType d_first, T init)
{
    return sequential::naive::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, T init)
{
    return sequential::naive::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation>
IterType inclusive_segmented_scan(IterType        first,
                                  IterType        last,
                                  IterType        d_first,
                                  BinaryOperation binary_op)
{
    using PairType = typename std::iterator_traits<IterType>::value_type;
    using FlagType = typename std::tuple_element<1, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");

    /*Sequential inclusive scan becomes the segmented variant by wrapping the
      passed binary_op into a new conditional binary operation. Assuming
      a addition with the running sum as operand x and the current value as
      operand y, resetting the sum to the value of y yields the correct result.
     */
    return sequential::naive::inclusive_scan(first,
                                             last,
                                             d_first,
                                             [binary_op](PairType x, PairType y)
                                             {
                                                 PairType result = y;
                                                 if (!y.second)
                                                 {
                                                     result.first =
                                                         binary_op(x.first, y.first);
                                                     /* Only required if additions are
                                                        reordered !
                                                      */
                                                     // if (x.second)
                                                     // {
                                                     //     result.second = x.second;
                                                     // }
                                                 }
                                                 return result;
                                             });
}

template<class IterType>
IterType inclusive_segmented_scan(IterType first, IterType last, IterType d_first)
{
    return sequential::naive::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_segmented_scan(IterType first, IterType last)
{
    return sequential::naive::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation, class T>
IterType exclusive_segmented_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<IterType>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to First pair type!");

    size_t    num_values = last - first;
    ValueType sum        = init;
    for (size_t i = 0; i < num_values; i++)
    {
        if (!first[i].second)
        {
            d_first[i].first = sum;
            sum              = binary_op(sum, first[i].first);
        }
        else
        {
            d_first[i].first = init;
            sum              = first[i].first;
        }
    }
    return first + num_values;
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, IterType d_first, T init)
{
    return sequential::naive::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, T init)
{
    return sequential::naive::exclusive_segmented_scan(
        first, last, first, init, std::plus<>());
}

}; // namespace naive
namespace updown
{
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class IterType, class BinaryOperation>
IterType inclusive_scan(IterType&       first,
                        IterType&       last,
                        IterType&       d_first,
                        BinaryOperation binary_op)
{
    size_t num_values = last - first;
    size_t step       = 1;
    // Up sweep

    if (std::distance(first, d_first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        for (size_t i = 0; i < num_values; i = i + step)
        {
            d_first[i + step - 1] =
                binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
        }
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));
    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
        for (size_t i = step; i - 1 < num_values - 2; i = i + step)
        {
            d_first[i + step / 2 - 1] =
                binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
        }
    }
    return d_first + num_values;
}

template<class IterType>
IterType inclusive_scan(IterType first, IterType last, IterType d_first)
{
    return sequential::updown::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_scan(IterType first, IterType last)
{
    return sequential::updown::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class IterType, class T, class BinaryOperation>
IterType exclusive_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;
    static_assert(std::is_same<ValueType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 1;

    // Up sweep
    if (std::distance(first, d_first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        for (size_t i = 0; i < num_values; i = i + step)
        {
            d_first[i + step - 1] =
                binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
        }
    }
    d_first[num_values - 1] = init;

    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            ValueType t                   = d_first[i + (1 << stage) - 1];
            d_first[i + (1 << stage) - 1] = d_first[i + (1 << (stage + 1)) - 1];
            d_first[i + (1 << (stage + 1)) - 1] =
                binary_op(t, d_first[i + (1 << (stage + 1)) - 1]);
        }
    }

    return d_first + num_values;
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, IterType d_first, T init)
{
    return sequential::updown::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, T init)
{
    return sequential::updown::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation>
IterType inclusive_segmented_scan(IterType        first,
                                  IterType        last,
                                  IterType        d_first,
                                  BinaryOperation binary_op)
{
    /*Up-Down inclusive scan becomes the segmented variant by wrapping the
      passed binary_op into a new conditional binary operation. Assuming
      a addition with the running sum as operand x and the current value as
      operand y, resetting the sum to the value of y yields the correct result.

      Copying of the flag is only required when executing non
      sequentially. Since the up-sweep works on operands over segment
      boundaries as the stages progress, segments already finished need to be
      marked as such. In these cases, the flag of the right operand is set
      to prevent further addition during later stages.

      For the down sweep phase unset flags represent unfinished work rather
      than actual segments.
    */
    using PairType = typename std::iterator_traits<IterType>::value_type;
    using FlagType = typename std::tuple_element<1, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");

    return sequential::updown::inclusive_scan(first,
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

template<class IterType>
IterType inclusive_segmented_scan(IterType first, IterType last, IterType d_first)
{
    return sequential::updown::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_segmented_scan(IterType first, IterType last)
{
    return sequential::updown::inclusive_segmented_scan(
        first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation, class T>
IterType exclusive_segmented_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    /* Due to the add-swap operation in the down sweep phase a simple wrapper
       of the binary operation is insufficient.
     */
    using PairType  = typename std::iterator_traits<IterType>::value_type;
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

    // First stage of the up-sweep fused with copy
    step = step * 2;
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t left = i + step / 2 - 1, right = i + step - 1;
        // Copy left operand to d_first.
        d_first[left].first = first[left].first;
        temp_flags[left]    = first[left].second;

        if (first[right].second)
        {
            // Copy if right flag is set.
            d_first[right].first = first[right].first;
            temp_flags[right]    = first[right].second;
        }
        else
        {
            // Add if right flag is not set.
            d_first[right].first = binary_op(first[left].first, first[right].first);
            // Right flag is copied from the left flag if left flag is set.
            temp_flags[right] =
                first[left].second ? first[left].second : first[right].second;
        }
    }

    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
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
    d_first[num_values - 1] = std::make_pair(init, 0);
    for (int stage = std::floor(std::log2(num_values)) - 1; stage > 0; stage--)
    {
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            /*Add-Swap Operation needs to be handled differently than with all the
              other variants. Flags from the previous phase remain unmodified.
              There are two cases to be observed.
             */
            ValueType t = d_first[left].first;
            if (not temp_flags[left])
            {
                /*Left operand is not a segment beginning:
                  Independent of the right operand, swap the left value
                  with the right and save the sum in the right operand.

                  The segment beginnings will be reset to the init value at
                  a later stage and are currently required to hold intermediate
                  results.
                 */
                d_first[left].first  = d_first[right].first;
                d_first[right].first = binary_op(t, d_first[right].first);
            }
            else
            {
                /*Left operand is a segment beginning:
                  Independent of the right operand, swap the operand values.
                  This rule moves the already correctly calculated values into
                  their right place.
                 */
                d_first[left].first  = d_first[right].first;
                d_first[right].first = t;
            }
        }
    }
    // Last stage of up-sweep meaning that stage = 0
    for (size_t i = 0; i < num_values; i = i + 2)
    {
        //        left = i + (1 << 0) - 1, right = i + (1 << (0 + 1)) - 1;
        size_t    left = i, right = i + 1;
        ValueType t = d_first[left].first;
        // Modified rules to cause segment starts to be overwritten with init.
        if (not first[left].second)
        {
            if (not first[right].second)
            {
                d_first[left].first  = d_first[right].first;
                d_first[right].first = binary_op(t, d_first[right].first);
            }
            else
            {
                d_first[left].first  = d_first[right].first;
                d_first[right].first = init;
            }
        }
        else
        {
            if (not first[right].second)
            {
                d_first[left].first  = init;
                d_first[right].first = t;
            }
            else
            {
                d_first[left].first  = init;
                d_first[right].first = init;
            }
        }
    }
    return first + num_values;
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, IterType d_first, T init)
{
    return sequential::updown::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, T init)
{
    return sequential::updown::exclusive_segmented_scan(
        first, last, first, init, std::plus<>());
}

}; // namespace updown
namespace tiled
{
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class IterType, class BinaryOperation>
IterType
inclusive_scan(IterType first, IterType last, IterType d_first, BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values - 1) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values - 1) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    std::vector<ValueType> temp(num_tiles + 1);

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = *(first + 1 + i * tile_size);
        for (size_t j = 2 + i * tile_size; j < 1 + (i + 1) * tile_size; j++)
        {
            temp[i] = binary_op(temp[i], *(first + j));
        }
    }

    // Phase 2: Intermediate Scan
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, binary_op);

    // Phase 3: Rescan
    for (size_t i = 0; i < num_tiles; i++)
    {

        std::exclusive_scan(first + 1 + i * tile_size,
                            first + 1 + (i + 1) * tile_size,
                            d_first + i * tile_size,
                            temp[i],
                            binary_op);
    }
    if (rem)
    {
        std::exclusive_scan(first + num_values - rem + 1,
                            first + num_values + 1,
                            d_first + num_values - rem,
                            temp[num_tiles],
                            binary_op);
    }
    return d_first + num_values;
}

template<class IterType>
IterType inclusive_scan(IterType first, IterType last, IterType d_first)
{
    return sequential::tiled::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_scan(IterType first, IterType last)
{
    return sequential::tiled::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class IterType, class T, class BinaryOperation>
IterType exclusive_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    std::vector<ValueType> temp(num_tiles + 1);

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = std::reduce(
            first + i * tile_size, first + (i + 1) * tile_size, init, binary_op);
    }

    // Phase 2: Intermediate Scan
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init, binary_op);

    // Phase 3: Rescan
    for (size_t i = 0; i < num_tiles; i++)
    {
        std::exclusive_scan(first + i * tile_size,
                            first + (i + 1) * tile_size,
                            d_first + i * tile_size,
                            temp[i],
                            binary_op);
    }
    if (rem)
    {
        std::exclusive_scan(first + num_values - rem,
                            first + num_values,
                            d_first + num_values - rem,
                            temp[num_tiles],
                            binary_op);
    }
    return d_first + num_values;
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, IterType d_first, T init)
{
    return sequential::tiled::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, T init)
{
    return sequential::tiled::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation>
IterType inclusive_segmented_scan(IterType        first,
                                  IterType        last,
                                  IterType        d_first,
                                  BinaryOperation binary_op)
{
    using PairType = typename std::iterator_traits<IterType>::value_type;

    return sequential::tiled::inclusive_scan(first,
                                             last,
                                             d_first,
                                             [binary_op](PairType x, PairType y)
                                             {
                                                 PairType result = y;
                                                 if (!y.second)
                                                 {
                                                     result.first =
                                                         binary_op(x.first, y.first);
                                                     if (x.second)
                                                     {
                                                         result.second = x.second;
                                                     }
                                                 }
                                                 return result;
                                             });
}

template<class IterType>
IterType inclusive_segmented_scan(IterType first, IterType last, IterType d_first)
{
    return sequential::tiled::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_segmented_scan(IterType first, IterType last)
{
    return sequential::tiled::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation, class T>
IterType exclusive_segmented_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<IterType>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to First pair type!");

    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    auto wrapped_bop = [binary_op](PairType x, PairType y)
    {
        PairType result = y;
        if (!y.second)
        {
            result.first = binary_op(x.first, y.first);
            // Since additions are reordered
            // flags need to be carried along
            // to indicate finished segments!
            if (x.second)
            {
                result.second = x.second;
            }
        }
        return result;
    };

    std::vector<PairType> temp(num_tiles + 1);

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = std::reduce(first + i * tile_size,
                              first + (i + 1) * tile_size,
                              std::make_pair(init, 0),
                              wrapped_bop);
        // temp[i] = std::make_pair(init, 0);
        // for (size_t j = i * tile_size; j < (i + 1) * tile_size; j++)
        // {
        //     if (not first[j].second)
        //     {
        //         temp[i].first = binary_op(temp[i].first, first[j].first);
        //     }
        //     else
        //     {
        //         temp[i].first = first[j].first;
        //     }
        // }
    }
    // Phase 2: Intermediate Scan
    std::cout << "temp"
              << ":" << std::endl;
    std::for_each(temp.begin(), temp.end(), [](auto x) { std::cout << x.first << ", "; });
    std::cout << std::endl;
    std::for_each(
        temp.begin(), temp.end(), [](auto x) { std::cout << x.second << ", "; });
    std::cout << std::endl;

    std::exclusive_scan(
        temp.begin(), temp.end(), temp.begin(), std::make_pair(init, 0), wrapped_bop);

    std::cout << "temp"
              << ":" << std::endl;
    std::for_each(temp.begin(), temp.end(), [](auto x) { std::cout << x.first << ", "; });
    std::cout << std::endl;
    std::for_each(
        temp.begin(), temp.end(), [](auto x) { std::cout << x.second << ", "; });
    std::cout << std::endl;

    std::cout << "first"
              << ":" << std::endl;
    std::for_each(
        first, first + num_values, [](auto x) { std::cout << x.first << ", "; });
    std::cout << std::endl;
    std::for_each(
        first, first + num_values, [](auto x) { std::cout << x.second << ", "; });
    std::cout << std::endl;

    // Phase 3: Rescan
    for (size_t i = 0; i < num_tiles; i++)
    {
        ValueType sum = temp[i].first;
        for (size_t j = i * tile_size; j < (i + 1) * tile_size; j++)
        {
            if (!first[j].second)
            {
                d_first[j].first = sum;
                sum              = binary_op(sum, first[j].first);
            }
            else
            {
                d_first[j].first = init;
                sum              = first[j].first;
            }
        }
    }
    if (rem)
    {
        ValueType sum = temp[num_tiles].first;
        for (size_t j = num_values - rem; j < num_values; j++)
        {
            if (!first[j].second)
            {
                d_first[j].first = sum;
                sum              = binary_op(sum, first[j].first);
            }
            else
            {
                d_first[j].first = init;
                sum              = first[j].first;
            }
        }
    }
    return d_first + num_values;
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, IterType d_first, T init)
{
    return sequential::tiled::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, T init)
{
    return sequential::tiled::exclusive_segmented_scan(
        first, last, first, init, std::plus<>());
}
}; // namespace tiled
}; // namespace sequential
