#pragma once

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <numeric>
namespace sequential
{
namespace tiled
{
// Controls the number of elements a tile has.
size_t tile_size = 4;
void   set_tile_size(size_t size) { tiled::tile_size = size; }

// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<typename InputIter, typename OutputIter, typename BinaryOperation>
OutputIter inclusive_scan(InputIter       first,
                          InputIter       last,
                          OutputIter      d_first,
                          BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<InputIter>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values - 1 < tile_size)
    {
        tile_size = num_values - 1;
    }
    size_t num_tiles = (num_values - 1) / tile_size;

    std::vector<ValueType> temp(num_tiles + 1);

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = *(first + 1 + i * tile_size);
        for (size_t j = 2 + i * tile_size; j < 1 + (i + 1) * tile_size; j++)
        {
            temp[i] = binary_op(temp[i], *(first + j));
        }
    }

    // Phase 2: Intermediate Scan
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, binary_op);
    d_first[0] = first[0];

    // Phase 3: Rescan
    for (size_t i = 0; i <= num_tiles; i++)
    {
        size_t    begin = 1 + i * tile_size, end = 1 + (i + 1) * tile_size;
        ValueType sum = temp[i];
        if (end > num_values)
        {
            end = num_values;
        }

        for (size_t j = begin; j < end; j++)
        {
            sum        = binary_op(sum, first[j]);
            d_first[j] = sum;
        }
    }
    return d_first + num_values;
}

template<typename InputIter, typename OutputIter>
OutputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first)
{
    return sequential::tiled::inclusive_scan(first, last, d_first, std::plus<>());
}

template<typename InputIter> InputIter inclusive_scan(InputIter first, InputIter last)
{
    return sequential::tiled::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename OutputIter, typename T, typename BinaryOperation>
OutputIter exclusive_scan(InputIter       first,
                          InputIter       last,
                          OutputIter      d_first,
                          T               init,
                          BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<InputIter>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values < tile_size)
    {
        tile_size = num_values;
    }
    size_t num_tiles = num_values / tile_size - 1;

    std::vector<ValueType> temp(num_tiles + 1);

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = std::reduce(
            first + i * tile_size, first + (i + 1) * tile_size, init, binary_op);
    }

    // Phase 2: Intermediate Scan
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init, binary_op);

    // Phase 3: Rescan

    for (size_t i = 0; i <= num_tiles; i++)
    {
        size_t begin = i * tile_size, end = (i + 1) * tile_size;
        if (end > num_values)
        {
            end = num_values;
        }

        ValueType sum = temp[i];
        for (size_t j = begin; j < end; j++)
        {
            ValueType temp = d_first[j];
            d_first[j]     = sum;
            sum            = binary_op(sum, temp);
        }
    }

    return d_first + num_values;
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter exclusive_scan(InputIter first, InputIter last, OutputIter d_first, T init)
{
    return sequential::tiled::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<typename InputIter, typename T>
InputIter exclusive_scan(InputIter first, InputIter last, T init)
{
    return sequential::tiled::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename OutputIter, typename BinaryOperation>
OutputIter inclusive_segmented_scan(InputIter       first,
                                    InputIter       last,
                                    OutputIter      d_first,
                                    BinaryOperation binary_op)
{
    using PairType = typename std::iterator_traits<InputIter>::value_type;

    return sequential::tiled::inclusive_scan(first,
                                             last,
                                             d_first,
                                             [binary_op](PairType x, PairType y)
                                             {
                                                 PairType result = y;
                                                 if (!y.second)
                                                 {
                                                     result.first =
                                                         binary_op(x.first, y.first);
                                                     if (x.second)
                                                     {
                                                         result.second = x.second;
                                                     }
                                                 }
                                                 return result;
                                             });
}

template<typename InputIter, typename OutputIter>
OutputIter inclusive_segmented_scan(InputIter first, InputIter last, OutputIter d_first)
{
    return sequential::tiled::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<typename InputIter>
InputIter inclusive_segmented_scan(InputIter first, InputIter last)
{
    return sequential::tiled::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<typename InputIter, typename OutputIter, typename BinaryOperation, typename T>
OutputIter exclusive_segmented_scan(InputIter       first,
                                    InputIter       last,
                                    OutputIter      d_first,
                                    T               identity,
                                    T               init,
                                    BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<InputIter>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to First pair type!");

    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values < tile_size)
    {
        tile_size = num_values;
    }
    size_t num_tiles = num_values / tile_size - 1;

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
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = std::reduce(first + i * tile_size,
                              first + (i + 1) * tile_size,
                              std::make_pair(identity, 0),
                              wrapped_bop);
    }

    // Phase 2: Intermediate Scan
    std::exclusive_scan(
        temp.begin(), temp.end(), temp.begin(), std::make_pair(identity, 0), wrapped_bop);

    // Phase 3: Rescan
    for (size_t i = 0; i <= num_tiles; i++)
    {
        size_t end = (i + 1) * tile_size;
        end        = end > num_values ? num_values : end;

        ValueType sum = binary_op(temp[i].first, init);
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
    }

    return d_first + num_values;
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter exclusive_segmented_scan(
    InputIter first, InputIter last, OutputIter d_first, T identity, T init)
{
    return sequential::tiled::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<typename InputIter, typename T>
InputIter exclusive_segmented_scan(InputIter first, InputIter last, T identity, T init)
{
    return sequential::tiled::exclusive_segmented_scan(
        first, last, first, identity, init, std::plus<>());
}
}; // namespace tiled
}; // namespace sequential
