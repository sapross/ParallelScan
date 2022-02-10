#pragma once
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <vector>

namespace _tbb
{
namespace tiled
{
// Controls the number of elements a tile has.
size_t tile_size = 4;
void   set_tile_size(size_t size) { tiled::tile_size = size; }
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
    using ValueType = typename std::iterator_traits<InputIt>::value_type;

    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values < tile_size)
    {
        tile_size = num_values;
    }
    size_t num_tiles = num_values / tile_size - 1;

    std::vector<ValueType> temp(num_tiles + 1);

    // Phase 1: Reduction om Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                        temp[i] = *(first + 1 + i * tile_size);
                        for (size_t j = 2 + i * tile_size; j < 1 + (i + 1) * tile_size; j++)
                        {
                            temp[i] = binary_op(temp[i], *(first + j));
                        }
                      });

    // Phase 2: Intermediate Scan (parallel)
    /*
        ToDo:
        Problem: exclusive the following exclusive scan requires an identity element for
       which y0 = id * z0 not an element for initialization y0 = *first
    */
    //_tbb::provided::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first,
    // binary_op);
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, binary_op);
    d_first[0] = temp[0];

    // Phase 3: Rescan on Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles + 1,
                      size_t(1),
                      [&](auto i)
                      {
                          size_t begin = 1 + i * tile_size, end = 1 + (i + 1) * tile_size;
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
                      });
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
    InputIt first, InputIt last, OutputIt d_first, T identity, T init, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    using ValueType   = typename std::iterator_traits<InputIt>::value_type;
    size_t num_values = last - first;
    size_t tile_size  = tiled::tile_size;
    if (num_values < tile_size)
    {
        tile_size = num_values;
    }
    size_t num_tiles = num_values / tile_size - 1;
    ;
    std::vector<InputType> temp(num_tiles + 1);

    // Phase 1: Reduction om Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                          temp[i] = *(first + i * tile_size);
                          for (size_t j = 1 + i * tile_size; j < (i + 1) * tile_size; j++)
                          {
                              temp[i] = binary_op(temp[i], *(first + j));
                          }
                      });

    // Phase 2: Intermediate Scan (sequential)

    _tbb::provided::exclusive_scan(
        temp.begin(), temp.end(), temp.begin(), identity, init, binary_op);

    // Phase 3: Rescan on Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles + 1,
                      size_t(1),
                      [&](auto i)
                      {
                          size_t begin = i * tile_size, end = (i + 1) * tile_size;
                          if (end > num_values)
                          {
                              end = num_values;
                          }

                          ValueType sum = temp[i];
                          for (size_t j = begin; j < end; j++)
                          {
                              ValueType temp = first[j];
                              d_first[j]     = sum;
                              sum            = binary_op(sum, temp);
                          }
                      });
    return d_first + num_values;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T identity, T init)
{
    return _tbb::tiled::exclusive_scan(first, last, d_first, identity, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T identity, T init)
{
    return _tbb::tiled::exclusive_scan(first, last, first, identity, init, std::plus<>());
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
OutputIt exclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  OutputIt        d_first,
                                  T               identity,
                                  T               init,
                                  BinaryOperation binary_op)
{
    using PairType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType  = typename std::tuple_element<1, PairType>::type;
    using ValueType = typename std::tuple_element<0, PairType>::type;
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

    // Phase 1: Reduction on Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles,
                      size_t(1),
                      [&](auto i)
                      {
                          temp[i] = *(first + i * tile_size);
                          for (size_t j = 1 + i * tile_size; j < (i + 1) * tile_size; j++)
                          {
                              temp[i] = wrapped_bop(temp[i], *(first + j));
                          }
                      });

    // Phase 2: Intermediate Scan (parallel)
    _tbb::provided::exclusive_scan(
        temp.begin(), temp.end(), temp.begin(), std::make_pair(ValueType(), FlagType()), std::make_pair(identity, FlagType()) , wrapped_bop);
    //std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), std::make_pair(init, false), wrapped_bop);

    // Phase 3: Rescan on Tiles (parallel)
    tbb::parallel_for(size_t(0),
                      num_tiles + 1,
                      size_t(1),
                      [&](auto i)
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
                      });
    return d_first + num_values;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, OutputIt d_first, T identity, T init)
{
    return _tbb::tiled::exclusive_segmented_scan(
        first, last, d_first, identity, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, T identity, T init)
{
    return _tbb::tiled::exclusive_segmented_scan(
        first, last, first, identity, init, std::plus<>());
}

}; // namespace tiled
}; // namespace _tbb
