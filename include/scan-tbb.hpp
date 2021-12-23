#pragma once

#include <vector>

#include <tbb/tbb.h>
#include <tbb/parallel_for.h>

namespace naive {
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt inclusive_scan(InputIt&        first,
                        InputIt&        last,
                        OutputIt&       d_first,
                        BinaryOperation binary_op)
{

}

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return naive::updown::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return naive::updown::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------
template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt exclusive_scan(InputIt&        first,
                        InputIt&        last,
                        OutputIt&       d_first,
                        BinaryOperation binary_op)
{
                            
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return naive::updown::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return naive::updown::exclusive_scan(first, last, first, init, std::plus<>());
}
}