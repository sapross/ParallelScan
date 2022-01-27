#pragma once

namespace openmp
{
namespace tiled
{
// Controls the number of elements a tile has.
size_t tile_size = 4;
void   set_tile_size(size_t size) { tiled::tile_size = size; }

// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class IterType, class BinaryOperation>
IterType
inclusive_scan(IterType first, IterType last, IterType d_first, BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values < tile_size)
    {
        tile_size = num_values;
    }
    size_t num_tiles = num_values / tile_size - 1;

    std::vector<ValueType> temp(num_tiles + 1);

// Phase 1: Reduction om Tiles (parallel)
#pragma omp parallel for
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = *(first + 1 + i * tile_size);
        for (size_t j = 2 + i * tile_size; j < 1 + (i + 1) * tile_size; j++)
        {
            temp[i] = binary_op(temp[i], *(first + j));
        }
    }

    // Phase 2: Intermediate Scan (parallel)
    openmp::provided::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first);

// Phase 3: Rescan on Tiles (parallel)
#pragma omp parallel for
    for (size_t i = 0; i <= num_tiles; i++)
    {
        size_t begin = 1 + i * tile_size, end = 1 + (i + 1) * tile_size;
        std::exclusive_scan(first + begin,
                            end > num_values + 1 ? first + num_values : first + end,
                            d_first + i * tile_size,
                            temp[i],
                            binary_op);
    }
    return d_first + num_values;
}

template<class IterType>
IterType inclusive_scan(IterType first, IterType last, IterType d_first)
{
    return openmp::tiled::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_scan(IterType first, IterType last)
{
    return openmp::tiled::inclusive_scan(first, last, first, std::plus<>());
}
// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class IterType, class T, class BinaryOperation>
IterType exclusive_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values < tile_size)
    {
        tile_size = num_values;
    }
    size_t num_tiles = num_values / tile_size - 1;

    std::vector<ValueType> temp(num_tiles + 1);

// Phase 1: Reduction
#pragma omp parallel for
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = std::reduce(
            first + i * tile_size, first + (i + 1) * tile_size, init, binary_op);
    }

    // Phase 2: Intermediate Scan

    openmp::provided::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init);

// Phase 3: Rescan
#pragma omp parallel for
    for (size_t i = 0; i <= num_tiles; i++)
    {
        size_t begin = i * tile_size, end = (i + 1) * tile_size;
        std::exclusive_scan(first + begin,
                            end > num_values ? first + num_values : first + end,
                            d_first + i * tile_size,
                            temp[i],
                            binary_op);
    }
    return d_first + num_values;
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, IterType d_first, T init)
{
    return openmp::tiled::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, T init)
{
    return openmp::tiled::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation>
IterType inclusive_segmented_scan(IterType        first,
                                  IterType        last,
                                  IterType        d_first,
                                  BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<IterType>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");

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
#pragma omp parallel for

    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = *(first + i * tile_size);
        for (size_t j = 1 + i * tile_size; j < (i + 1) * tile_size; j++)
        {
            temp[i] = wrapped_bop(temp[i], first[j]);
        }
    }

    // Phase 2: Intermediate Scan (sequential)
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, wrapped_bop);

// Phase 3: Rescan on Tiles (parallel)
#pragma omp parallel for
    for (size_t i = 0; i <= num_tiles; i++)
    {
        size_t end = (i + 1) * tile_size;
        end        = end > num_values ? num_values : end;

        ValueType sum = temp[i].first;
        for (size_t j = i * tile_size; j < end; j++)
        {
            ValueType temp = first[j].first;
            if (!first[j].second and j != 0)
            {
                sum = binary_op(sum, temp);
            }
            else
            {
                sum = temp;
            }
            d_first[j].first = sum;
        }
    }
    return d_first + num_values;
}

template<class IterType>
IterType inclusive_segmented_scan(IterType first, IterType last, IterType d_first)
{
    return openmp::tiled::inclusive_segmented_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_segmented_scan(IterType first, IterType last)
{
    return openmp::tiled::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class IterType, class BinaryOperation, class T>
IterType exclusive_segmented_scan(IterType        first,
                                  IterType        last,
                                  IterType        d_first,
                                  T               identity,
                                  T               init,
                                  BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<IterType>::value_type;
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
#pragma omp parallel for
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
#pragma omp parallel for
    for (size_t i = 0; i <= num_tiles; i++)
    {
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
    }

    return d_first + num_values;
}

template<class IterType, class T>
IterType exclusive_segmented_scan(
    IterType first, IterType last, IterType d_first, T identity, T init)
{
    return openmp::tiled::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, T identity, T init)
{
    return openmp::tiled::exclusive_segmented_scan(
        first, last, first, identity, init, std::plus<>());
}
} // namespace tiled
} // namespace openmp
