#pragma once
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <vector>

namespace _tbb
{
namespace tiled
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

    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values - 1) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values - 1) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    std::vector<InputType> temp(num_tiles + 1);

    // Phase 1: Reduction om Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                          temp[i] = std::reduce(first + 1 + i * tile_size,
                                                first + 1 + (i + 1) * tile_size,
                                                InputType(),
                                                binary_op);
                      });

    // Phase 2: Intermediate Scan (sequential)
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, binary_op);

    // Phase 3: Rescan on Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                          std::exclusive_scan(first + 1 + i * tile_size,
                                              first + 1 + (i + 1) * tile_size,
                                              d_first + i * tile_size,
                                              temp[i],
                                              binary_op);
                      });
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

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::tiled::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return _tbb::tiled::inclusive_scan(first, last, first, std::plus<>());
}

// // ----------------------------------------------------------------------------------
// //  Exclusive Scan
// // ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(
    InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values - 1) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values - 1) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    std::vector<InputType> temp(num_tiles + 1);

    // Phase 1: Reduction om Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                          temp[i] = std::reduce(first + i * tile_size,
                                                first + (i + 1) * tile_size,
                                                InputType(),
                                                binary_op);
                      });

    // Phase 2: Intermediate Scan (sequential)
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init, binary_op);

    // Phase 3: Rescan on Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                          std::exclusive_scan(first + i * tile_size,
                                              first + (i + 1) * tile_size,
                                              d_first + i * tile_size,
                                              temp[i],
                                              binary_op);
                      });
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

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return _tbb::tiled::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return _tbb::tiled::exclusive_scan(first, last, first, init, std::plus<>());
}

// // ----------------------------------------------------------------------------------
// //  Inclusive Segmented Scan
// // ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class BinaryOperation>
OutputIt inclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  OutputIt        d_first,
                                  BinaryOperation binary_op)
{
    using PairType   = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::tuple_element<1, PairType>::type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<PairType, OutputType>::value,
                  "Input type must be convertible to output type!");

    return _tbb::tiled::inclusive_scan(first,
                                       last,
                                       d_first,
                                       [binary_op](PairType x, PairType y)
                                       {
                                           PairType result = y;
                                           if (!y.second)
                                           {
                                               result.first = binary_op(x.first, y.first);
                                               if (x.second)
                                               {
                                                   result.second = x.second;
                                               }
                                           }
                                           return result;
                                       });
}

template<class InputIt, class OutputIt>
OutputIt inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::tiled::inclusive_segmented_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_segmented_scan(InputIt first, InputIt last)
{
    return _tbb::tiled::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class BinaryOperation, class T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
{
    using PairType   = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::tuple_element<1, PairType>::type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<PairType, OutputType>::value,
                  "Input type must be convertible to output type!");

    _tbb::tiled::exclusive_scan(first,
                                last,
                                d_first,
                                std::make_pair(init, 0),
                                [binary_op](PairType x, PairType y)
                                {
                                    PairType result = y;
                                    if (!y.second)
                                    {
                                        result.first = binary_op(x.first, y.first);
                                        if (x.second)
                                        {
                                            result.second = x.second;
                                        }
                                    }
                                    return result;
                                });
    while (first != last)
    {
        if ((*first).second)
        {
            d_first->first = init;
        }
        first++;
        d_first++;
    }
    return d_first;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return _tbb::tiled::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, T init)
{
    return _tbb::tiled::exclusive_segmented_scan(first, last, first, init, std::plus<>());
}

}; // namespace tiled

}; // namespace _tbb
