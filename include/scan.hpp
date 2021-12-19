#pragma once

#include <algorithm>
#include <functional>

#include <cstddef>
#include <numeric>
#include <stdint.h>
namespace naive
{
namespace sequential
{
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

template<class ForwardIt1, class ForwardIt2, class T>
ForwardIt2 exclusive_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first, T init)
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
        sum += *i;
    }
    return d_first;
}

}; // namespace sequential
}; // namespace naive
