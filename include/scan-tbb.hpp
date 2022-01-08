#pragma once

#include <vector>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>

// required for distance
#include <iterator>
//for identity
#include <functional>

namespace _tbb
{
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt inclusive_scan(InputIt&        first,
                        InputIt&        last,
                        OutputIt&       d_first,
                        BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    using range_type = tbb::blocked_range<size_t>;
    tbb::parallel_scan(range_type(0, std::distance(first, last)), 0,
		[&](const range_type &r, OutputType sum, bool is_final_scan) {
			OutputType tmp = sum;
			for (size_t i = r.begin(); i < r.end(); ++i)
            {
				tmp = binary_op(tmp, first[i]);
				if (is_final_scan)
				    d_first[i] = tmp;
			}
			return tmp;
		},
		[&](const InputType &a, const InputType &b) {
			return binary_op(a, b);
		});
    return d_first + std::distance(first, last);
}

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return _tbb::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------
template<class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(InputIt&        first,
                        InputIt&        last,
                        OutputIt&       d_first,
                        T               init,
                        BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    using range_type = tbb::blocked_range<size_t>;
    tbb::parallel_scan(range_type(0, std::distance(first, last)), 0,
		[&](const range_type &r, OutputType sum, bool is_final_scan) {
			OutputType tmp = sum;
			for (size_t i = r.begin(); i < r.end(); ++i)
            {
				tmp = binary_op(tmp, first[i]);
				if (is_final_scan)
				    d_first[i+1] = tmp;
			}
			return tmp;
		},
		[&](const InputType &a, const InputType &b) {
			return binary_op(a, b);
		});
    return d_first + std::distance(first, last);
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return _tbb::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return _tbb::exclusive_scan(first, last, first, init, std::plus<>());
}
} // namespace naive