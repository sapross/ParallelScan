#pragma once

#include <algorithm>
#include <functional>
#include <iomanip>
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
template<typename InputIter, typename OutputIter, typename BinaryOperation>
OutputIter inclusive_scan(InputIter       first,
                          InputIter       last,
                          OutputIter      d_first,
                          BinaryOperation binary_op)
{
    return std::inclusive_scan(first, last, d_first, binary_op);
}

template<typename InputIter, typename OutputIter>
InputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first)
{
    return sequential::naive::inclusive_scan(first, last, d_first, std::plus<>());
}

template<typename InputIter> InputIter inclusive_scan(InputIter first, InputIter last)
{
    return sequential::naive::inclusive_scan(first, last, first, std::plus<>());
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

    return std::exclusive_scan(first, last, d_first, init, binary_op);
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter exclusive_scan(InputIter first, InputIter last, OutputIter d_first, T init)
{
    return sequential::naive::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<typename InputIter, typename T>
InputIter exclusive_scan(InputIter first, InputIter last, T init)
{
    return sequential::naive::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename OutputIter, typename BinaryOperation>
OutputIter inclusive_segmented_scan(InputIter       first,
                                    InputIter       last,
                                    OutputIter      d_first,
                                    BinaryOperation binary_op)
{
    using PairType = typename std::iterator_traits<InputIter>::value_type;
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

template<typename InputIter, typename OutputIter>
OutputIter inclusive_segmented_scan(InputIter first, InputIter last, OutputIter d_first)
{
    return sequential::naive::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<typename InputIter>
InputIter inclusive_segmented_scan(InputIter first, InputIter last)
{
    return sequential::naive::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename OutputIter, typename BinaryOperation, typename T>
OutputIter exclusive_segmented_scan(InputIter       first,
                                    InputIter       last,
                                    OutputIter      d_first,
                                    T               init,
                                    BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<InputIter>::value_type;
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
        T temp = first[i].first;
        if (!first[i].second)
        {
            d_first[i].first = sum;
            sum              = binary_op(sum, temp);
        }
        else
        {
            d_first[i].first = init;
            sum              = binary_op(temp, init);
        }
    }

    return d_first + num_values;
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter
exclusive_segmented_scan(InputIter first, InputIter last, OutputIter d_first, T init)
{
    return sequential::naive::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<typename InputIter, typename T>
InputIter exclusive_segmented_scan(InputIter first, InputIter last, T init)
{
    using PairType  = typename std::iterator_traits<InputIter>::value_type;
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
        T temp = first[i].first;
        if (!first[i].second)
        {
            first[i].first = sum;
            sum            = sum + temp;
        }
        else
        {
            first[i].first = init;
            sum            = temp + init;
        }
    }

    return last;
}

}; // namespace naive
}; // namespace sequential
