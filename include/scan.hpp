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
template<class ForwardIt1, class ForwardIt2, class BinaryOperation>
ForwardIt2 inclusive_scan(ForwardIt1      first,
                          ForwardIt1      last,
                          ForwardIt2      d_first,
                          BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<ForwardIt1>::value_type;
    using OutputType = typename std::iterator_traits<ForwardIt2>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    InputType sum = InputType(0);
    for (auto i = first; i != last; i++)
    {
        sum          = binary_op(sum, *i);
        *(d_first++) = sum;
    }
    return d_first;
}

template<class ForwardIt1, class ForwardIt2>
ForwardIt2 inclusive_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first)
{
    return naive::sequential::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class ForwardIt1> ForwardIt1 inclusive_scan(ForwardIt1 first, ForwardIt1 last)
{
    return naive::sequential::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class ForwardIt1, class ForwardIt2, class T, class BinaryOperation>
ForwardIt2 exclusive_scan(ForwardIt1      first,
                          ForwardIt1      last,
                          ForwardIt2      d_first,
                          T               init,
                          BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<ForwardIt1>::value_type;
    using OutputType = typename std::iterator_traits<ForwardIt2>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    T sum = init;
    for (auto i = first; i != last; i++)
    {
        *(d_first++) = sum;
        sum          = binary_op(sum, *i);
    }
    return d_first;
}

template<class ForwardIt1, class ForwardIt2, class T>
ForwardIt2 exclusive_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first, T init)
{
    return naive::sequential::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class ForwardIt1, class T>
ForwardIt1 exclusive_scan(ForwardIt1 first, ForwardIt1 last, T init)
{
    return naive::sequential::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class ForwardIt1, class ForwardIt2, class ForwardIt3, class BinaryOperation>
ForwardIt2 inclusive_segmented_scan(ForwardIt1      first,
                                    ForwardIt1      last,
                                    ForwardIt3      b_first,
                                    ForwardIt2      d_first,
                                    BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<ForwardIt1>::value_type;
    using BoolType   = typename std::iterator_traits<ForwardIt3>::value_type;
    using OutputType = typename std::iterator_traits<ForwardIt2>::value_type;
    static_assert(std::is_convertible<BoolType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    InputType sum = InputType(0);

    for (auto i = first; i != last; i++)
    {
        if (*(b_first++))
        {
            sum = binary_op(sum, *i);
        }
        else
        {
            sum = *i;
        }
        *(d_first++) = sum;
    }
    return d_first;
}

template<class ForwardIt1, class ForwardIt2, class ForwardIt3>
ForwardIt2 inclusive_segmented_scan(ForwardIt1 first,
                                    ForwardIt1 last,
                                    ForwardIt3 b_first,
                                    ForwardIt2 d_first)
{
    return naive::sequential::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<class ForwardIt1, class ForwardIt3>
ForwardIt1 inclusive_segmented_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt3 b_first)
{
    return naive::sequential::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class ForwardIt1,
         class ForwardIt2,
         class ForwardIt3,
         class BinaryOperation,
         class T>
ForwardIt2 exclusive_segmented_scan(ForwardIt1      first,
                                    ForwardIt1      last,
                                    ForwardIt3      b_first,
                                    ForwardIt2      d_first,
                                    T               init,
                                    BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<ForwardIt1>::value_type;
    using BoolType   = typename std::iterator_traits<ForwardIt3>::value_type;
    using OutputType = typename std::iterator_traits<ForwardIt2>::value_type;
    static_assert(std::is_convertible<BoolType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    InputType sum = init;

    for (auto i = first; i != last; i++)
    {
        if (*(b_first++))
        {
            sum = binary_op(sum, *i);
        }
        else
        {
            sum = *i;
        }
        *(d_first++) = sum;
    }
    return d_first;
}

template<class ForwardIt1, class ForwardIt2, class ForwardIt3, class T>
ForwardIt2 exclusive_segmented_scan(
    ForwardIt1 first, ForwardIt1 last, ForwardIt3 b_first, ForwardIt2 d_first, T init)
{
    return naive::sequential::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<class ForwardIt1, class ForwardIt3, class T>
ForwardIt1
exclusive_segmented_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt3 b_first, T init)
{
    return naive::sequential::exclusive_segmented_scan(
        first, last, first, init, std::plus<>());
}

}; // namespace sequential
}; // namespace naive
