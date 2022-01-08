#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>

namespace naive
{
namespace sequential
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

    return std::inclusive_scan(first, last, d_first, binary_op);
}

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return naive::sequential::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return naive::sequential::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(
    InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    return std::exclusive_scan(first, last, d_first, init, binary_op);
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return naive::sequential::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return naive::sequential::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

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

    return naive::sequential::inclusive_scan(first,
                                             last,
                                             d_first,
                                             [binary_op](PairType x, PairType y)
                                             {
                                                 PairType result;
                                                 result.second = 0;
                                                 if (!y.second)
                                                 {
                                                     result.first =
                                                         binary_op(x.first, y.first);
                                                 }
                                                 else
                                                 {
                                                     result.first = y.first;
                                                 }
                                                 return result;
                                             });
}

template<class InputIt, class OutputIt>
OutputIt inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return naive::sequential::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_segmented_scan(InputIt first, InputIt last)
{
    return naive::sequential::inclusive_segmented_scan(first, last, first, std::plus<>());
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

    naive::sequential::exclusive_scan(first,
                                      last,
                                      d_first,
                                      std::make_pair(init, 0),
                                      [init, binary_op](PairType x, PairType y)
                                      {
                                          PairType result;
                                          result.second = 0;
                                          if (!y.second)
                                          {
                                              result.first = binary_op(x.first, y.first);
                                          }
                                          else
                                          {
                                              result.first = y.first;
                                          }
                                          return result;
                                      });
    while (first != last)
    {
        if (first->second)
        {
            d_first->first = 0;
        }
        first++;
        d_first++;
    }
    return first;
}

template<class InputIt, class OutputIt, class T>
OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T init)
{
    return naive::sequential::exclusive_segmented_scan(
        first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, T init)
{
    return naive::sequential::exclusive_segmented_scan(
        first, last, first, init, std::plus<>());
}

}; // namespace sequential
namespace updown
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

    size_t num_values = last - first;
    size_t step       = 1;
    // Up sweep

    if (std::distance(first, d_first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        for (size_t i = 0; i < num_values; i = i + step)
        {
            d_first[i + step - 1] =
                binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
        }
    }
    step = 1 << (size_t)(std::floor(std::log2(num_values)));
    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
        for (size_t i = step; i - 1 < num_values - 2; i = i + step)
        {
            d_first[i + step / 2 - 1] =
                binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
        }
    }
    return d_first + num_values;
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

template<class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(
    InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 1;
    // Up sweep

    if (std::distance(first, d_first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        for (size_t i = 0; i < num_values; i = i + step)
        {
            d_first[i + step - 1] =
                binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
        }
    }
    d_first[num_values - 1] = init;
    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            OutputType t                        = d_first[i + (1 << stage) - 1];
            d_first[i + (1 << stage) - 1]       = d_first[i + (1 << (stage + 1)) - 1];
            d_first[i + (1 << (stage + 1)) - 1] = t + d_first[i + (1 << (stage + 1)) - 1];
        }
    }
    return d_first + num_values;
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

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class FlagIt, class BinaryOperation>
OutputIt inclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  FlagIt          flag_first,
                                  OutputIt        d_first,
                                  BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::iterator_traits<FlagIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    size_t num_values = last - first;
    size_t step       = 1;
    // Up sweep
    std::cout << "Input:" << std::endl;
    std::for_each(first, last, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;
    std::cout << "Flags:" << std::endl;
    std::for_each(
        flag_first, flag_first + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;

    if (std::distance(first, d_first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        for (size_t i = 0; i < num_values; i = i + step)
        {
            if (!flag_first[i + step - 1])
            {
                d_first[i + step - 1] =
                    binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
            }
            if (flag_first[i + step / 2 - 1])
            {
                flag_first[i + step - 1] = flag_first[i + step / 2 - 1];
            }
        }
    }
    std::cout << "After up sweep:" << std::endl;
    std::for_each(d_first, d_first + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;

    step = 1 << (size_t)(std::floor(std::log2(num_values)));
    for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
    {
        step = step / 2;
        for (size_t i = step; i - 1 < num_values - 2; i = i + step)
        {
            if (!flag_first[i + step / 2 - 1])
            {

                d_first[i + step / 2 - 1] =
                    binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
            }
            if (flag_first[i - 1])
            {
                flag_first[i + step / 2 - 1] = flag_first[i - 1];
            }
        }
    }
    std::cout << "After down sweep:" << std::endl;
    std::for_each(d_first, d_first + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;

    return d_first + num_values;
}

template<class InputIt, class OutputIt, class FlagIt>
OutputIt
inclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first, OutputIt d_first)
{
    return naive::updown::inclusive_segmented_scan(
        first, last, flag_first, d_first, std::plus<>());
}

template<class InputIt, class FlagIt>
InputIt inclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first)
{
    return naive::updown::inclusive_segmented_scan(
        first, last, flag_first, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class FlagIt, class BinaryOperation, class T>
OutputIt exclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  FlagIt          flag_first,
                                  OutputIt        d_first,
                                  T               init,
                                  BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::iterator_traits<FlagIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    size_t num_values = last - first;
    size_t step       = 1;

    std::vector<FlagType> temp_flag(num_values);
    std::copy(flag_first, flag_first + num_values, temp_flag.begin());
    auto flag_it = temp_flag.begin();

    std::cout << "Input:" << std::endl;
    std::for_each(first, last, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;
    std::cout << "Flags:" << std::endl;
    std::for_each(flag_it, flag_it + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;

    // Up sweep
    if (std::distance(first, d_first) != 0)
    {
        std::copy(first, last, d_first);
    }
    for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
    {
        step = step * 2;
        for (size_t i = 0; i < num_values; i = i + step)
        {
            if (!flag_it[i + step - 1])
            {
                d_first[i + step - 1] =
                    binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
            }
            if (flag_it[i + step / 2 - 1])
            {
                flag_it[i + step - 1] = flag_it[i + step / 2 - 1];
            }
        }
    }
    d_first[num_values - 1] = init;
    std::copy(flag_first, flag_first + num_values, flag_it);

    std::cout << "After up sweep:" << std::endl;
    std::for_each(d_first, d_first + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;
    std::cout << "Flags:" << std::endl;
    std::for_each(flag_it, flag_it + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;

    for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
    {
        for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
        {
            size_t left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;

            OutputType t  = d_first[left];
            OutputType ft = flag_first[left];

            if (!flag_it[left])
            {
                d_first[left]  = d_first[right];
                d_first[right] = d_first[right] + t;
            }
            else
            {
                if (!flag_it[right])
                {
                    d_first[left]  = d_first[right];
                    d_first[right] = t;
                }
            }
        }
    }

    for (size_t i = 0; i < num_values; i++)
    {
        if (flag_first[i])
        {
            d_first[i] = init;
        }
    }

    std::cout << "After down sweep:" << std::endl;
    std::for_each(d_first, d_first + num_values, [](auto x) { std::cout << x << ", "; });
    std::cout << std::endl;

    return d_first + num_values;
}

template<class InputIt, class OutputIt, class FlagIt, class T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, FlagIt flag_first, OutputIt d_first, T init)
{
    return naive::updown::exclusive_segmented_scan(
        first, last, flag_first, d_first, init, std::plus<>());
}

template<class InputIt, class FlagIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first, T init)
{
    return naive::updown::exclusive_segmented_scan(
        first, last, flag_first, first, init, std::plus<>());
}

}; // namespace updown
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

    // std::cout << "Tiled:" << std::endl;
    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values - 1) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values - 1) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    // std::cout << "num_values:" << num_values << std::endl;
    // std::cout << "tile_size:" << tile_size << std::endl;
    // std::cout << "num_tiles:" << num_tiles << std::endl;
    // std::cout << "rem:" << rem << std::endl;

    std::vector<InputType> temp(num_tiles + 1, 0);

    // std::cout << "temp: ";
    // std::for_each(temp.begin(), temp.end(), [](auto e) { std::cout << e << ", "; });
    // std::cout << std::endl;

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        // std::cout << "temp[" << i << "] = std::reduce(" << 1 + i * tile_size << ","
        //           << 1 + (i + 1) * tile_size << ",InputType(0),binary_op);" <<
        //           std::endl;

        temp[i] = std::reduce(first + 1 + i * tile_size,
                              first + 1 + (i + 1) * tile_size,
                              InputType(0),
                              binary_op);
    }
    // std::cout << "temp: ";
    // std::for_each(temp.begin(), temp.end(), [](auto e) { std::cout << e << ", "; });
    // std::cout << std::endl;

    // Phase 2: Intermediate Scan
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, binary_op);

    // std::cout << "temp: ";
    // std::for_each(temp.begin(), temp.end(), [](auto e) { std::cout << e << ", "; });
    // std::cout << std::endl;

    // Phase 3: Rescan
    for (size_t i = 0; i < num_tiles; i++)
    {
        // std::cout << "std::exclusive_scan(" << 1 + i * tile_size << ","
        //           << 1 + (i + 1) * tile_size << ", " << i * tile_size << "," << temp[i]
        //           << ",InputType(0),binary_op);" << std::endl;

        std::exclusive_scan(first + 1 + i * tile_size,
                            first + 1 + (i + 1) * tile_size,
                            d_first + i * tile_size,
                            temp[i],
                            binary_op);
    }
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
    return naive::tiled::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
{
    return naive::tiled::inclusive_scan(first, last, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class T, class BinaryOperation>
OutputIt exclusive_scan(
    InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    // std::cout << "Tiled:" << std::endl;
    size_t num_values = last - first;
    size_t tile_size  = 4;
    tile_size         = (num_values) > tile_size ? tile_size : 1;
    size_t num_tiles  = (num_values) / tile_size;
    size_t rem        = num_values - tile_size * num_tiles;

    // std::cout << "num_values:" << num_values << std::endl;
    // std::cout << "tile_size:" << tile_size << std::endl;
    // std::cout << "num_tiles:" << num_tiles << std::endl;
    // std::cout << "rem:" << rem << std::endl;

    std::vector<InputType> temp(num_tiles + 1, 0);

    // std::cout << "temp: ";
    // std::for_each(temp.begin(), temp.end(), [](auto e) { std::cout << e << ", "; });
    // std::cout << std::endl;

    // Phase 1: Reduction
    for (size_t i = 0; i < num_tiles; i++)
    {
        // std::cout << "temp[" << i << "] = std::reduce(" << 1 + i * tile_size << ","
        //           << (i + 1) * tile_size << ",InputType(0),binary_op);" << std::endl;

        temp[i] = std::reduce(
            first + i * tile_size, first + (i + 1) * tile_size, InputType(0), binary_op);
    }
    // std::cout << "temp: ";
    // std::for_each(temp.begin(), temp.end(), [](auto e) { std::cout << e << ", "; });
    // std::cout << std::endl;

    // Phase 2: Intermediate Scan
    std::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init, binary_op);

    // std::cout << "temp: ";
    // std::for_each(temp.begin(), temp.end(), [](auto e) { std::cout << e << ", "; });
    // std::cout << std::endl;

    // Phase 3: Rescan
    for (size_t i = 0; i < num_tiles; i++)
    {
        // std::cout << "std::exclusive_scan(" << i * tile_size << "," << (i + 1) *
        // tile_size
        //           << ", " << i * tile_size << "," << temp[i]
        //           << ",InputType(0),binary_op);" << std::endl;

        std::exclusive_scan(first + i * tile_size,
                            first + (i + 1) * tile_size,
                            d_first + i * tile_size,
                            temp[i],
                            binary_op);
    }
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
    return naive::tiled::exclusive_scan(first, last, d_first, init, std::plus<>());
}

template<class InputIt, class T>
InputIt exclusive_scan(InputIt first, InputIt last, T init)
{
    return naive::tiled::exclusive_scan(first, last, first, init, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Inclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class FlagIt, class BinaryOperation>
OutputIt inclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  FlagIt          flag_first,
                                  OutputIt        d_first,
                                  BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::iterator_traits<FlagIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");

    return naive::tiled::inclusive_scan(first,
                                        last,
                                        d_first,
                                        [&flag_first, binary_op](InputType x, InputType y)
                                        {
                                            if (!*(++flag_first))
                                            {
                                                return binary_op(x, y);
                                            }
                                            else
                                            {
                                                return y;
                                            }
                                        });
}

template<class InputIt, class OutputIt, class FlagIt>
OutputIt
inclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first, OutputIt d_first)
{
    return naive::tiled::inclusive_segmented_scan(
        first, last, flag_first, d_first, std::plus<>());
}

template<class InputIt, class FlagIt>
InputIt inclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first)
{
    return naive::tiled::inclusive_segmented_scan(
        first, last, flag_first, first, std::plus<>());
}

// ----------------------------------------------------------------------------------
//  Exclusive Segmented Scan
// ----------------------------------------------------------------------------------

template<class InputIt, class OutputIt, class FlagIt, class BinaryOperation, class T>
OutputIt exclusive_segmented_scan(InputIt         first,
                                  InputIt         last,
                                  FlagIt          flag_first,
                                  OutputIt        d_first,
                                  T               init,
                                  BinaryOperation binary_op)
{
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using FlagType   = typename std::iterator_traits<FlagIt>::value_type;
    using OutputType = typename std::iterator_traits<OutputIt>::value_type;
    static_assert(std::is_convertible<FlagType, bool>::value,
                  "Second Input Iterator type must be convertible to bool!");
    static_assert(std::is_convertible<InputType, OutputType>::value,
                  "Input type must be convertible to output type!");
    static_assert(std::is_same<InputType, T>::value,
                  "Underlying input and init type have to be the same!");

    return naive::tiled::exclusive_scan(
        first,
        last,
        d_first,
        init,
        [&flag_first, &init, binary_op](InputType x, InputType y)
        {
            if (!*(++flag_first))
            {
                return binary_op(x, y);
            }
            else
            {
                return init;
            }
        });
}

template<class InputIt, class OutputIt, class FlagIt, class T>
OutputIt exclusive_segmented_scan(
    InputIt first, InputIt last, FlagIt flag_first, OutputIt d_first, T init)
{
    return naive::tiled::exclusive_segmented_scan(
        first, last, flag_first, d_first, init, std::plus<>());
}

template<class InputIt, class FlagIt, class T>
InputIt exclusive_segmented_scan(InputIt first, InputIt last, FlagIt flag_first, T init)
{
    return naive::tiled::exclusive_segmented_scan(
        first, last, flag_first, first, init, std::plus<>());
}

}; // namespace tiled
}; // namespace naive
