#pragma once

namespace openmp
{
namespace provided
{
template<class IterType>
IterType inclusive_scan(IterType first, IterType last, IterType d_first)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;

    size_t    num_values = last - first;
    ValueType sum        = 0;
#pragma omp      parallel for reduction(inscan, + : sum)
    for (size_t i = 0; i < num_values; ++i)
    {
        sum += *(first + i);
#pragma omp scan inclusive(sum)
        *(d_first + i) = sum;
    }
    return d_first;
}

template<class IterType> IterType inclusive_scan(IterType first, IterType last)
{
    return openmp::provided::inclusive_scan(first, last, first);
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, IterType d_first, T init)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;

    size_t    num_values = last - first;
    ValueType sum        = init;
#pragma omp      parallel for reduction(inscan, + : sum)
    for (size_t i = 0; i < num_values; ++i)
    {
        *(d_first + i) = sum;
#pragma omp scan exclusive(sum)
        sum += *(first + i);
    }
    return d_first;
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, T init)
{
    return openmp::provided::exclusive_scan(first, last, first, init);
}

/*Unfortunately, it is not possible to use the provided function
to implement an inclusive segmented scan*/
template<class IterType>
IterType inclusive_segmented_scan(IterType first, IterType last, IterType d_first)
{
    using PairType = typename std::iterator_traits<IterType>::value_type;
    using FlagType = typename std::tuple_element<1, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");

    size_t num_values = last - first;
    int    sum        = 0;
#pragma omp      parallel for reduction(inscan, + : sum)
    for (size_t i = 0; i < num_values; ++i)
    {
        if (!((*(first + i)).second))
        {
            sum += (*(first + i)).first;
        }

        else
        {
            sum = (*(first + i)).first;
        }
#pragma omp scan inclusive(sum)
        (*(d_first + i)).first = sum;
    }
    return d_first;
}

template<class IterType> IterType inclusive_segmented_scan(IterType first, IterType last)
{
    return openmp::provided::inclusive_segmented_scan(first, last, first);
}

/*Unfortunately, it is not possible to use the provided function
to implement an exclusive segmented scan*/
/*Unfortunately, it is not possible to use the provided function
to implement an exclusive segmented scan*/
template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, IterType d_first, T init)
{
    using PairType  = typename std::iterator_traits<IterType>::value_type;
    using ValueType = typename std::tuple_element<0, PairType>::type;

    size_t    num_values = last - first;
    ValueType sum        = init;
#pragma omp      parallel for reduction(inscan, + : sum)
    for (size_t i = 0; i < num_values; ++i)
    {
        (*(d_first + i)).first = sum.first;
#pragma omp scan exclusive(sum)
        if (!((*(first + i)).second))
        {
            sum += (*(first + i)).first;
        }

        else
        {
            sum = init + (*(first + i)).first;
        }
    }
    return d_first;
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, T init)
{
    return openmp::provided::exclusive_segmented_scan(first, last, first, init);
}
} // namespace provided
namespace updown
{
// ----------------------------------------------------------------------------------
//  Inclusive Scan
// ----------------------------------------------------------------------------------
template<class IterType, class BinaryOperation>
IterType inclusive_scan(IterType&       first,
                        IterType&       last,
                        IterType&       d_first,
                        BinaryOperation binary_op)
{
    size_t num_values = last - first;
    size_t step       = 2;
// Up sweep

// First stage of the up sweep fused with copy.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t left = i + step / 2 - 1, right = i + step - 1;
        d_first[left]  = first[left];
        d_first[right] = binary_op(first[left], first[right]);
    }

    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            d_first[right] = binary_op(d_first[left], d_first[right]);
        }
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));
    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
#pragma omp parallel for
        for (size_t i = step; i < (num_values - 1); i = i + step)
        {
            d_first[i + step / 2 - 1] =
                binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
        }
    }
    return d_first + num_values;
}

template<class IterType>
IterType inclusive_scan(IterType first, IterType last, IterType d_first)
{
    return openmp::updown::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_scan(IterType first, IterType last)
{
    return openmp::updown::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class IterType, class T, class BinaryOperation>
IterType exclusive_scan(
    IterType first, IterType last, IterType d_first, T init, BinaryOperation binary_op)
{
    using ValueType = typename std::iterator_traits<IterType>::value_type;
    static_assert(std::is_same<ValueType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 2;
// Up sweep

// First stage of the up sweep fused with copy.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t left = i + step / 2 - 1, right = i + step - 1;
        d_first[left]  = first[left];
        d_first[right] = binary_op(first[left], first[right]);
    }
    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            d_first[right] = binary_op(d_first[left], d_first[right]);
        }
    }

    d_first[num_values - 1] = init;

    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t    left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            ValueType val_left = d_first[left], val_right = d_first[right];
            d_first[left]  = val_right;
            d_first[right] = binary_op(val_left, val_right);
        }
    }
    return d_first + num_values;
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, IterType d_first, T init)
{
    return openmp::updown::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_scan(IterType first, IterType last, T init)
{
    return openmp::updown::exclusive_scan(first, last, first, init, std::plus<>());
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
    using PairType = typename std::iterator_traits<IterType>::value_type;
    using FlagType = typename std::tuple_element<1, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");

    return openmp::updown::inclusive_scan(first,
                                          last,
                                          d_first,
                                          [binary_op](PairType x, PairType y)
                                          {
                                              PairType result = y;
                                              if (!y.second)
                                              {
                                                  result.first =
                                                      binary_op(x.first, y.first);
                                                  // Since additions are reordered
                                                  // flags need to be carried along
                                                  // to indicate finished segments!
                                                  if (x.second)
                                                  {
                                                      result.second = x.second;
                                                  }
                                              }
                                              return result;
                                          });
}

template<class IterType>
IterType inclusive_segmented_scan(IterType first, IterType last, IterType d_first)
{
    return openmp::updown::inclusive_segmented_scan(first, last, d_first, std::plus<>());
}

template<class IterType> IterType inclusive_segmented_scan(IterType first, IterType last)
{
    return openmp::updown::inclusive_segmented_scan(first, last, first, std::plus<>());
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
    /* Due to the add-swap operation in the down sweep phase a simple wrapper
       of the binary operation is insufficient.
     */
    using PairType  = typename std::iterator_traits<IterType>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second pair type must be convertible to bool!");
    static_assert(std::is_convertible<T, ValueType>::value,
                  "Init must be convertible to First pair type!");

    size_t                num_values = last - first;
    size_t                step       = 1;
    std::vector<FlagType> temp_flags(num_values);
    /* Up sweep
       Essentially the same operation as with the conditional wrapper but
       now unpacked. This step is hence equal to the segmented inclusive
       up-down sweeping scan.
    */

    // First stage of the up sweep fused with copy
    step = step * 2;
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + step)
    {
        size_t   left = i + step / 2 - 1, right = i + step - 1;
        PairType val_left = first[left], val_right = first[right];

        // Copy flags into temp_flags.
        temp_flags[left] = val_left.second;
        // If left operand is segment start, mark right operand as finished.
        temp_flags[right] = val_left.second ? val_left.second : val_right.second;
        if (not val_right.second)
        {
            val_right.first = binary_op(val_left.first, val_right.first);
        }

        d_first[left]  = val_left;
        d_first[right] = val_right;
    }

    // Remainder stages of the up sweep.
    for (size_t stage = 1; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + step)
        {
            size_t left = i + step / 2 - 1, right = i + step - 1;
            if (!temp_flags[right])
            {
                d_first[right].first =
                    binary_op(d_first[left].first, d_first[right].first);
                if (temp_flags[left])
                {
                    temp_flags[right] = temp_flags[left];
                }
            }
        }
    }

    d_first[num_values - 1].first = identity;

    // Down sweep
    for (int stage = std::floor(std::log2(num_values)) - 1; stage > 0; stage--)
    {
#pragma omp parallel for
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
            /*Add-Swap Operation needs to be handled differently than with all the
              other variants. Flags from the previous phase remain unmodified.
              There are two cases to be observed.
             */
            ValueType val_left = d_first[left].first, val_right = d_first[right].first;
            if (not temp_flags[left])
            {
                /*Left operand is not a segment beginning:
                  Independent of the right operand, swap the left value
                  with the right and save the sum in the right operand.

                  The segment beginnings will be reset to the init value at
                  a later stage and are currently required to hold intermediate
                  results.
                 */
                d_first[left].first  = val_right;
                d_first[right].first = binary_op(val_left, val_right);
            }
            else
            {
                /*Left operand is a segment beginning:
                  Independent of the right operand, swap the operand values.
                  This rule moves the already correctly calculated values into
                  their right place.
                 */
                d_first[left].first  = val_right;
                d_first[right].first = val_left;
            }
        }
    }

// Last stage of down-sweep meaning that stage = 0
// This stage is fused with a cleanup of the segment beginnings.
#pragma omp parallel for
    for (size_t i = 0; i < num_values; i = i + 2)
    {
        size_t    left = i, right = i + 1;
        ValueType val_left = d_first[left].first, val_right = d_first[right].first;
        ValueType temp = val_left;

        // Modified rules to cause segment starts to be overwritten with init.
        if (not first[left].second and i != 0)
        {
            if (not first[right].second)
            {
                val_left  = val_right;
                val_right = binary_op(temp, val_right);
            }
            else
            {
                val_left  = d_first[right].first;
                val_right = identity;
            }
        }
        else
        {
            if (not first[right].second)
            {
                val_left  = identity;
                val_right = temp;
            }
            else
            {
                val_left  = identity;
                val_right = identity;
            }
        }
        d_first[left].first  = binary_op(init, val_left);
        d_first[right].first = binary_op(init, val_right);
    }
    return first + num_values;
}

template<class IterType, class T>
IterType exclusive_segmented_scan(
    IterType first, IterType last, IterType d_first, T identity, T init)
{
    return openmp::updown::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<class IterType, class T>
IterType exclusive_segmented_scan(IterType first, IterType last, T identity, T init)
{
    return openmp::updown::exclusive_segmented_scan(
        first, last, first, identity, init, std::plus<>());
}

} // namespace updown
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
    size_t tile_size  = 4;
    tile_size         = (num_values - 1) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values - 1) / tile_size;

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

    // std::cout << "Tiled:" << std::endl;
    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values) / tile_size;

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
#pragma omp parallel for
    for (size_t i = 0; i < num_tiles; i++)
    {
        temp[i] = std::reduce(first + i * tile_size,
                              first + (i + 1) * tile_size,
                              std::make_pair(0, 0),
                              wrapped_bop);
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
            if (!first[j].second)
            {
                d_first[j].first = sum;
                sum              = binary_op(sum, temp);
            }
            else
            {
                d_first[j].first = 0;
                sum              = temp;
            }
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
