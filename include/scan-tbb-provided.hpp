#pragma once
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <vector>

namespace _tbb
{
namespace provided
{
// ----------------------------------------------------------------------------------
//  Partitioners for tbb
//  Valid for parallel_scan : auto_partitioner, simple_partitioner
// ----------------------------------------------------------------------------------
auto scan_part = tbb::auto_partitioner();
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<typename InputIt,
         typename OutputIt,
         typename BinaryOperation,
         typename T,
         typename Partitioner>
OutputIt inclusive_scan(InputIt         first,
                        InputIt         last,
                        OutputIt        d_first,
                        T               identity,
                        BinaryOperation binary_op,
                        Partitioner     part)
{
    using InputType   = typename std::iterator_traits<InputIt>::value_type;
    using range_type  = tbb::blocked_range<size_t>;
    size_t num_values = last - first;
    tbb::parallel_scan(
        range_type(size_t(0), num_values),
        identity,
        [&](const range_type& r, InputType sum, bool is_final_scan)
        {
            InputType tmp = sum;
#pragma omp for simd
            for (size_t i = r.begin(); i < r.end(); ++i)
            {
                tmp = binary_op(tmp, first[i]);
                if (is_final_scan)
                    d_first[i] = tmp;
            }
            return tmp;
        },
        [&](const InputType a, const InputType b) { return binary_op(a, b); },
        part);
    return d_first + num_values;
}

template<typename InputIt, typename OutputIt, typename BinaryOperation, typename T>
OutputIt inclusive_scan(
    InputIt first, InputIt last, OutputIt d_first, T identity, BinaryOperation binary_op)
{
    return _tbb::provided::inclusive_scan(
        first, last, d_first, identity, binary_op, tbb::auto_partitioner());
}

template<typename InputIt, typename OutputIt, typename T>
OutputIt inclusive_scan(InputIt first, InputIt last, T identity, OutputIt d_first)
{
    return _tbb::provided::inclusive_scan(
        first, last, d_first, identity, std::plus<>(), tbb::auto_partitioner());
}

template<typename InputIt, typename T>
InputIt inclusive_scan(InputIt first, InputIt last, T identity)
{
    return _tbb::provided::inclusive_scan(first, last, first, identity, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------
template<typename InputIt,
         typename OutputIt,
         typename T,
         typename BinaryOperation,
         typename Partitioner>
OutputIt exclusive_scan(InputIt         first,
                        InputIt         last,
                        OutputIt        d_first,
                        T               identity,
                        T               init,
                        BinaryOperation binary_op,
                        Partitioner     part)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    using range_type  = tbb::blocked_range<size_t>;
    using ValueType   = typename std::iterator_traits<InputIt>::value_type;
    size_t num_values = last - first;
    tbb::parallel_scan(
        range_type(size_t(0), num_values),
        identity,
        [&](const range_type& r, ValueType sum, bool is_final_scan)
        {
#pragma omp for simd
            for (size_t i = r.begin(); i < r.end(); ++i)
            {
                if (i == 0)
                    sum = init; // binary_op(sum, init);
                ValueType tmp = first[i];
                if (is_final_scan)
                    d_first[i] = sum;
                sum = binary_op(sum, tmp);
            }
            return sum;
        },
        [&](const InputType& a, const InputType& b) { return binary_op(a, b); },
        part);
    d_first[0] = init;
    return d_first + num_values;
}

template<typename InputIt, typename OutputIt, typename T, typename BinaryOperation>
OutputIt exclusive_scan(InputIt         first,
                        InputIt         last,
                        OutputIt        d_first,
                        T               identity,
                        T               init,
                        BinaryOperation binary_op)
{
    return _tbb::provided::exclusive_scan(
        first, last, d_first, identity, init, binary_op, tbb::auto_partitioner());
}

template<typename InputIt, typename OutputIt, typename T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T identity, T init)
{
    return _tbb::provided::exclusive_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<typename InputIt, typename T>
InputIt exclusive_scan(InputIt first, InputIt last, T identity, T init)
{
    return _tbb::provided::exclusive_scan(
        first, last, first, identity, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIt,
         typename OutputIt,
         typename BinaryOperation,
         typename T,
         typename Partitioner>
OutputIt inclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  OutputIt        d_first,
                                  T               identity,
                                  BinaryOperation binary_op,
                                  Partitioner     part)
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
    _tbb::provided::inclusive_scan(
        first,
        last,
        d_first,
        std::make_pair(identity, 0),
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
        },
        part);

    return d_first + (last - first);
}

template<typename InputIt, typename OutputIt, typename BinaryOperation, typename T>
OutputIt inclusive_segmented_scan(
    InputIt first, InputIt last, OutputIt d_first, T identity, BinaryOperation binary_op)
{
    return _tbb::provided::inclusive_segmented_scan(
        first, last, d_first, identity, binary_op, tbb::auto_partitioner());
}

template<typename InputIt, typename OutputIt, typename T>
OutputIt
inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T identity)
{
    return _tbb::provided::inclusive_segmented_scan(
        first, last, d_first, identity, std::plus<>());
}

template<typename InputIt, typename T>
InputIt inclusive_segmented_scan(InputIt first, InputIt last, T identity)
{
    return _tbb::provided::inclusive_segmented_scan(
        first, last, first, identity, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIt,
         typename OutputIt,
         typename BinaryOperation,
         typename T,
         typename Partitioner>
OutputIt exclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  OutputIt        d_first,
                                  T               identity,
                                  T               init,
                                  BinaryOperation binary_op,
                                  Partitioner     part)
{
    using PairType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be converible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to first pair type");

    size_t num_values = last - first;
    using range_type  = tbb::blocked_range<size_t>;
    tbb::parallel_scan(
        range_type(size_t(0), num_values),
        std::make_pair(identity, FlagType()),
        [&](const range_type& r, PairType sum, bool is_final_scan)
        {
#pragma omp for simd
            for (size_t i = r.begin(); i < r.end(); ++i)
            {
                ValueType temp = first[i].first;

                if (i == 0)
                {
                    sum.first = binary_op(sum.first, init);
                }
                if (!first[i].second)
                {
                    if (is_final_scan)
                        d_first[i] = sum;
                    sum.first = binary_op(sum.first, temp);
                }
                else
                {
                    if (is_final_scan)
                        d_first[i].first = init;
                    sum.first  = binary_op(init, temp);
                    sum.second = first[i].second;
                }
            }
            return sum;
        },
        [&](const PairType& a, const PairType& b)
        {
            PairType result = b;
            if (!b.second)
                result.first = binary_op(a.first, result.first);
            return result;
        },
        part);
    d_first[0].first = init;

    return d_first + num_values;
}

template<typename InputIt, typename OutputIt, typename BinaryOperation, typename T>
OutputIt exclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  OutputIt        d_first,
                                  T               identity,
                                  T               init,
                                  BinaryOperation binary_op)
{
    return _tbb::provided::exclusive_segmented_scan(
        first, last, d_first, identity, init, binary_op, tbb::auto_partitioner());
}

template<typename InputIt, typename OutputIt, typename T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, OutputIt d_first, T identity, T init)
{
    return _tbb::provided::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<typename InputIt, typename T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, T identity, T init)
{
    return _tbb::provided::exclusive_segmented_scan(
        first, last, first, identity, init, std::plus<>());
}

} // namespace provided
}; // namespace _tbb
