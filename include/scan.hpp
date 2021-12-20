#pragma once

#include <algorithm>
#include <functional>
#include <numeric>

namespace naive
{
namespace sequential
{

// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt
inclusive_scan(InputIt first, InputIt last, OutputIt d_first, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    return std::inclusive_scan(first, last, d_first, binary_op);
}

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return naive::sequential::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return naive::sequential::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

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

    return std::exclusive_scan(first, last, d_first, init, binary_op);
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return naive::sequential::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return naive::sequential::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class FlagIt, class BinaryOperation>
OutputIt inclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  FlagIt          flag_first,
                                  OutputIt        d_first,
                                  BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::iterator_traits<FlagIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    return naive::sequential::inclusive_scan(
        first,
        last,
        d_first,
        [&flag_first, binary_op](InputType x, InputType y)
        {
            if (!*(++flag_first))
            {
                return binary_op(x, y);
            }
            else
            {
                return y;
            }
        });
}

template<class InputIt, class OutputIt, class FlagIt>
OutputIt
inclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first, OutputIt d_first)
{
    return naive::sequential::inclusive_segmented_scan(
        first, last, flag_first, d_first, std::plus<>());
}

template<class InputIt, class FlagIt>
InputIt inclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first)
{
    return naive::sequential::inclusive_segmented_scan(
        first, last, flag_first, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class FlagIt, class BinaryOperation, class T>
OutputIt exclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  FlagIt          flag_first,
                                  OutputIt        d_first,
                                  T               init,
                                  BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::iterator_traits<FlagIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    return naive::sequential::exclusive_scan(
        first,
        last,
        d_first,
        init,
        [&flag_first, &init, binary_op](InputType x, InputType y)
        {
            if (!*(++flag_first))
            {
                return binary_op(x, y);
            }
            else
            {
                return init;
            }
        });
}

template<class InputIt, class OutputIt, class FlagIt, class T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, FlagIt flag_first, OutputIt d_first, T init)
{
    return naive::sequential::exclusive_segmented_scan(
        first, last, flag_first, d_first, init, std::plus<>());
}

template<class InputIt, class FlagIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first, T init)
{
    return naive::sequential::exclusive_segmented_scan(
        first, last, flag_first, first, init, std::plus<>());
}

}; // namespace sequential
}; // namespace naive
