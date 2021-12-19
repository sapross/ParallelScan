#pragma once

#include <algorithm>
#include <numeric>

#include <cstddef>
#include <stdint.h>

namespace naive
{
namespace sequential
{
template<class ForwardIt1, class ForwardIt2>
ForwardIt2 inclusive_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first)
{
    using InputType = typename std::iterator_traits<ForwardIt1>::value_type;
    InputType sum   = InputType(0);
    for (auto i = first; i != last; i++)
    {
        sum += *i;
        *(d_first++) = sum;
    }
    return d_first;
}
} // namespace sequential
}; // namespace naive
