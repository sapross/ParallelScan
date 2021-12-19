#pragma once

#include <cstddef>
#include <stdint.h>

namespace naive
{
namespace sequential
{
template<class ForwardIt1, class ForwardIt2>
ForwardIt2 inclusive_scan(ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first);
}
}; // namespace naive
