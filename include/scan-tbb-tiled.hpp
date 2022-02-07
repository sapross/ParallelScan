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
    auto temp1 = temp;
    //std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init, binary_op);
    _tbb::provided::exclusive_scan(temp1.begin(), temp1.end(), temp.begin(), init, binary_op);
    // std::cout << "STL result" << std::endl;
    //     std::for_each(temp.begin(),
    //                   temp.end(),
    //                   [](auto x) { std::cout << std::setw(2) << x << ", "; });
    //     std::cout << std::endl;
    //     std::cout << "tbb result" << std::endl;
    //     std::for_each(temp1.begin(),
    //                   temp1.end(),
    //                   [](auto x) { std::cout << std::setw(2) << x << ", "; });
    //     std::cout << std::endl;

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
    InputIt first, InputIt last, OutputIt d_first,T identity, T init, BinaryOperation binary_op)
{
    using PairType   = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::tuple_element<1, PairType>::type;
    using ValueType  = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Input type must be convertible to output type!");

    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values) / tile_size;

    auto wrapped_bop = [binary_op](PairType x, PairType y)
    {
        PairType result = y;
        if (!y.second)
        {
            result.first = binary_op(x.first, y.first);
            // Since additions are reordered
            // flags need to be carried along
            // to indicate finished segments!
            if (x.second)
            {
                result.second = x.second;
            }
        }
        return result;
    };

    std::vector<PairType> temp(num_tiles + 1);

// Phase 1: Reduction

    tbb::parallel_for(size_t(0), num_tiles, size_t(1),
                           [&](auto i)
                           {
                               temp[i] = std::reduce(first + i * tile_size,
                                                     first + (i + 1) * tile_size,
                                                     std::make_pair(identity, 0),
                                                     wrapped_bop);
                           });

// Phase 2: Intermediate Scan
    std::exclusive_scan(
       temp.begin(), temp.end(), temp.begin(), std::make_pair(identity, 0), wrapped_bop);

// Phase 3: Rescan
    tbb::parallel_for(size_t(0), num_tiles + 1, size_t(1),
    [&](auto i){
        size_t end = (i + 1) * tile_size;
        end        = end > num_values ? num_values : end;

        ValueType sum = binary_op(init, temp[i].first);
        for (size_t j = i * tile_size; j < end; j++)
        {
            ValueType temp = first[j].first;
            if (!first[j].second)
            {
                d_first[j].first = sum;
                sum              = binary_op(sum, temp);
            }
            else
            {
                d_first[j].first = init;
                sum              = binary_op(init, temp);
            }
        }
    });
    return d_first + num_values;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T identity, T init)
{
    return _tbb::tiled::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, T identity, T init)
{
    return _tbb::tiled::exclusive_segmented_scan(first, last, first, identity, init, std::plus<>());
}

}; // namespace tiled

}; // namespace _tbb
