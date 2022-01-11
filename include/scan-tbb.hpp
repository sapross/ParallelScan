#pragma once

#include <vector>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>

// required for distance
#include <iterator>

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
		                [&](const range_type &r, OutputType sum, bool is_final_scan)-> OutputType {
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
			                return binary_op(a, b);});
    return d_first + std::distance(first, last);
}

template<class InputIt, class OutputIt>
OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt> 
InputIt inclusive_scan(InputIt first, InputIt last)
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
    d_first[0] = init;
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

    /*Sequential inclusive scan becomes the segmented variant by wrapping the
      passed binary_op into a new conditional binary operation. Assuming
      a addition with the running sum as operand x and the current value as
      operand y, resetting the sum to the value of y yields the correct result.
     */
    using InputType  = typename std::iterator_traits<InputIt>::value_type;
    using range_type = tbb::blocked_range<size_t>;
    tbb::parallel_scan(range_type(0, std::distance(first, last)), 0,
		                [&](const range_type &r, PairType sum, bool is_final_scan) -> PairType {
			                PairType tmp = sum;
			                // for (size_t i = r.begin(); i < r.end(); ++i)
                            // {
			                // 	tmp = OutputType(binary_op(tmp.first, first[i].first),0);
			                // 	if (is_final_scan)
			                // 	    d_first[i].first = tmp.first;
			                // }
			                return tmp;
		                },
		                [&](const PairType &a, const PairType &b) {
			                //return InputType(binary_op(a.first, b.first),0);});
                            return a;});
    return d_first + std::distance(first, last);
}

template<class InputIt, class OutputIt>
OutputIt inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first)
{
    return _tbb::inclusive_segmented_scan(
        first, last, d_first, std::plus<>());
}

template<class InputIt> InputIt inclusive_segmented_scan(InputIt first, InputIt last)
{
    return _tbb::inclusive_segmented_scan(first, last, first, std::plus<>());
}

// // ----------------------------------------------------------------------------------
// //  Exclusive Segmented Scan
// // ----------------------------------------------------------------------------------

// template<class InputIt, class OutputIt, class BinaryOperation, class T>
// OutputIt exclusive_segmented_scan(
//     InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
// {
//     using PairType   = typename std::iterator_traits<InputIt>::value_type;
//     using FlagType   = typename std::tuple_element<1, PairType>::type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<FlagType, bool>::value,
//                   "Second Input Iterator type must be convertible to bool!");
//     static_assert(std::is_convertible<PairType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     /*Sequential exclusive scan becomes the segmented variant by wrapping the
//       passed binary_op into a new conditional binary like with inclusive scan.
//     */
//                  _tbb::exclusive_scan(first,
//                                       last,
//                                       d_first,
//                                       std::make_pair(init, 0),
//                                       [binary_op](PairType x, PairType y)
//                                       {
//                                           PairType result = y;
//                                           if (!y.second)
//                                           {
//                                               result.first = binary_op(x.first, y.first);
//                                               /* Only required if additions are
//                                                  reordered!
//                                               */
//                                               // if (x.second)
//                                               // {
//                                               //     result.second = x.second;
//                                               // }
//                                           }
//                                           return result;
//                                       });

//     // Reset of segment beginnings to initial value.
//     // Using only an operand wrapper it is not possible to omit this step!
//     // See implementation under numeric.h
//     while (first != last)
//     {
//         if (first->second)
//         {
//             d_first->first = init;
//         }
//         first++;
//         d_first++;
//     }
//     return first;
// }

// template<class InputIt, class OutputIt, class T>
// OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T init)
// {
//     return _tbb::exclusive_segmented_scan(
//         first, last, d_first, init, std::plus<>());
// }

// template<class InputIt, class T>
// InputIt exclusive_segmented_scan(InputIt first, InputIt last, T init)
// {
//     return _tbb::exclusive_segmented_scan(
//         first, last, first, init, std::plus<>());
// }
// namespace updown
// {
// // ----------------------------------------------------------------------------------
// //  Inclusive Scan
// // ----------------------------------------------------------------------------------
// template<class InputIt, class OutputIt, class BinaryOperation>
// OutputIt inclusive_scan(InputIt&        first,
//                         InputIt&        last,
//                         OutputIt&       d_first,
//                         BinaryOperation binary_op)
// {
//     using InputType  = typename std::iterator_traits<InputIt>::value_type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<InputType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     size_t num_values = last - first;
//     size_t step       = 1;
//     // Up sweep

//     if (std::distance(first, d_first) != 0)
//     {
//         std::copy(first, last, d_first);
//     }
//     for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
//     {
//         step = step * 2;
//         for (size_t i = 0; i < num_values; i = i + step)
//         {
//             d_first[i + step - 1] =
//                 binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
//         }
//     }
//     step = 1 << (size_t)(std::floor(std::log2(num_values)));
//     for (int stage = std::floor(std::log2(num_values - 2)); stage > 0; stage--)
//     {
//         step = step / 2;
//         for (size_t i = step; i - 1 < num_values - 2; i = i + step)
//         {
//             d_first[i + step / 2 - 1] =
//                 binary_op(d_first[i - 1], d_first[i + step / 2 - 1]);
//         }
//     }
//     return d_first + num_values;
// }

// template<class InputIt, class OutputIt>
// OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
// {
//     return naive::updown::inclusive_scan(first, last, d_first, std::plus<>());
// }

// template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
// {
//     return naive::updown::inclusive_scan(first, last, first, std::plus<>());
// }

// // ----------------------------------------------------------------------------------
// //  Exclusive Scan
// // ----------------------------------------------------------------------------------

// // ToDo: how should an Up-Down Sweep look like in _tbb?

// template<class InputIt, class OutputIt, class T, class BinaryOperation>
// OutputIt exclusive_scan(
//     InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
// {
//     using InputType  = typename std::iterator_traits<InputIt>::value_type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<InputType, OutputType>::value,
//                   "Input type must be convertible to output type!");
//     static_assert(std::is_same<InputType, T>::value,
//                   "Underlying input and init type have to be the same!");

//     size_t num_values = last - first;
//     size_t step       = 1;

//     // Up sweep
//     if (std::distance(first, d_first) != 0)
//     {
//         std::copy(first, last, d_first);
//     }
//     for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
//     {
//         step = step * 2;
//         for (size_t i = 0; i < num_values; i = i + step)
//         {
//             d_first[i + step - 1] =
//                 binary_op(d_first[i + step / 2 - 1], d_first[i + step - 1]);
//         }
//     }
//     d_first[num_values - 1] = init;

//     for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
//     {
//         for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
//         {
//             OutputType t                  = d_first[i + (1 << stage) - 1];
//             d_first[i + (1 << stage) - 1] = d_first[i + (1 << (stage + 1)) - 1];
//             d_first[i + (1 << (stage + 1)) - 1] =
//                 binary_op(t, d_first[i + (1 << (stage + 1)) - 1]);
//         }
//     }

//     return d_first + num_values;
// }

// template<class InputIt, class OutputIt, class T>
// OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
// {
//     return _tbb::updown::exclusive_scan(first, last, d_first, init, std::plus<>());
// }

// template<class InputIt, class T>
// InputIt exclusive_scan(InputIt first, InputIt last, T init)
// {
//     return _tbb::updown::exclusive_scan(first, last, first, init, std::plus<>());
// }

// // ----------------------------------------------------------------------------------
// //  Inclusive Segmented Scan
// // ----------------------------------------------------------------------------------

// template<class InputIt, class OutputIt, class BinaryOperation>
// OutputIt inclusive_segmented_scan(InputIt         first,
//                                   InputIt         last,
//                                   OutputIt        d_first,
//                                   BinaryOperation binary_op)
// {
//     /*Up-Down inclusive scan becomes the segmented variant by wrapping the
//       passed binary_op into a new conditional binary operation. Assuming
//       a addition with the running sum as operand x and the current value as
//       operand y, resetting the sum to the value of y yields the correct result.

//       Copying of the flag is only required when executing non
//       sequentially. Since the up-sweep works on operands over segment
//       boundaries as the stages progress, segments already finished need to be
//       marked as such. In these cases, the flag of the right operand is set
//       to prevent further addition during later stages.

//       For the down sweep phase unset flags represent unfinished work rather
//       than actual segments.
//     */
//     using PairType   = typename std::iterator_traits<InputIt>::value_type;
//     using FlagType   = typename std::tuple_element<1, PairType>::type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<FlagType, bool>::value,
//                   "Second Input Iterator type must be convertible to bool!");
//     static_assert(std::is_convertible<PairType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     return _tbb::updown::inclusive_scan(first,
//                                          last,
//                                          d_first,
//                                          [binary_op](PairType x, PairType y)
//                                          {
//                                              PairType result = y;
//                                              if (!y.second)
//                                              {
//                                                  result.first =
//                                                      binary_op(x.first, y.first);
//                                                  // Since additions are reordered
//                                                  // flags need to be carried along
//                                                  // to indicate finished segments!
//                                                  if (x.second)
//                                                  {
//                                                      result.second = x.second;
//                                                  }
//                                              }
//                                              return result;
//                                          });
// }

// template<class InputIt, class OutputIt>
// OutputIt inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first)
// {
//     return _tbb::updown::inclusive_segmented_scan(first, last, d_first, std::plus<>());
// }

// template<class InputIt> InputIt inclusive_segmented_scan(InputIt first, InputIt last)
// {
//     return _tbb::updown::inclusive_segmented_scan(first, last, first, std::plus<>());
// }

// // ----------------------------------------------------------------------------------
// //  Exclusive Segmented Scan
// // ----------------------------------------------------------------------------------

// template<class InputIt, class OutputIt, class BinaryOperation, class T>
// OutputIt exclusive_segmented_scan(
//     InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
// {
//     /* Due to the add-swap operation in the down sweep phase a simple wrapper
//        of the binary operation is insufficient.
//      */
//     using PairType   = typename std::iterator_traits<InputIt>::value_type;
//     using FlagType   = typename std::tuple_element<1, PairType>::type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<FlagType, bool>::value,
//                   "Second Input Iterator type must be convertible to bool!");
//     static_assert(std::is_convertible<PairType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     size_t num_values = last - first;
//     size_t step       = 1;

//     if (std::distance(first, d_first) != 0)
//     {
//         std::copy(first, last, d_first);
//     }

//     /* Up sweep
//        Essentially the same operation as with the conditional wrapper but
//        now unpacked. This step is hence equal to the segmented inclusive
//        up-down sweeping scan.
//     */
//     for (size_t stage = 0; stage < std::floor(std::log2(num_values)); stage++)
//     {
//         step = step * 2;
//         for (size_t i = 0; i < num_values; i = i + step)
//         {
//             size_t left = i + step / 2 - 1, right = i + step - 1;
//             if (!d_first[right].second)
//             {
//                 d_first[right].first =
//                     binary_op(d_first[left].first, d_first[right].first);
//                 if (d_first[left].second)
//                 {
//                     d_first[right].second = d_first[left].second;
//                 }
//             }
//         }
//     }
//     d_first[num_values - 1] = std::make_pair(init, 0);
//     for (int stage = std::floor(std::log2(num_values)) - 1; stage >= 0; stage--)
//     {
//         for (size_t i = 0; i < num_values; i = i + (1 << (stage + 1)))
//         {
//             size_t left = i + (1 << stage) - 1, right = i + (1 << (stage + 1)) - 1;
//             /*Add-Swap Operation needs to be handled differently than with all the
//               other variants. Flags from the previous phase remain unmodified.
//               There are two cases to be observed.
//              */
//             if (not d_first[left].second)
//             {
//                 /*Left operand is not a segment beginning:
//                   Independent of the right operand, swap the left value
//                   with the right and save the sum in the right operand.

//                   The segment beginnings will be reset to the init value at
//                   a later stage and are currently required to hold intermediate
//                   results.
//                  */
//                 OutputType t         = d_first[left];
//                 d_first[left].first  = d_first[right].first;
//                 d_first[right].first = binary_op(t.first, d_first[right].first);
//             }
//             else
//             {
//                 /*Left operand is a segment beginning:
//                   Independent of the right operand, swap the operand values.
//                   This rule moves the already correctly calculated values into
//                   their right place.
//                  */
//                 OutputType t         = d_first[left];
//                 d_first[left].first  = d_first[right].first;
//                 d_first[right].first = t.first;
//             }
//         }
//     }
//     // Like with all exclusives variants, this step cannot be omitted or
//     // fused with any previous operation.
//     while (first != last)
//     {
//         if (first->second)
//         {
//             d_first->first = init;
//         }
//         first++;
//         d_first++;
//     }
//     return first;
// }

// template<class InputIt, class OutputIt, class T>
// OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T init)
// {
//     return _tbb::updown::exclusive_segmented_scan(
//         first, last, d_first, init, std::plus<>());
// }

// template<class InputIt, class T>
// InputIt exclusive_segmented_scan(InputIt first, InputIt last, T init)
// {
//     return _tbb::updown::exclusive_segmented_scan(
//         first, last, first, init, std::plus<>());
// }

// }; // namespace updown
// namespace tiled
// {
// // ----------------------------------------------------------------------------------
// //  Inclusive Scan
// // ----------------------------------------------------------------------------------
// template<class InputIt, class OutputIt, class BinaryOperation>
// OutputIt
// inclusive_scan(InputIt first, InputIt last, OutputIt d_first, BinaryOperation binary_op)
// {
//     using InputType  = typename std::iterator_traits<InputIt>::value_type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<InputType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     size_t num_values = last - first;
//     size_t tile_size  = 4;
//     tile_size         = (num_values - 1) > tile_size ? tile_size : 1;
//     size_t num_tiles  = (num_values - 1) / tile_size;
//     size_t rem        = num_values - tile_size * num_tiles;

//     std::vector<InputType> temp(num_tiles + 1);

//     // Phase 1: Reduction
//     for (size_t i = 0; i < num_tiles; i++)
//     {
//         temp[i] = *(first + 1 + i * tile_size);
//         for (size_t j = 2 + i * tile_size; j < 1 + (i + 1) * tile_size; j++)
//         {
//             temp[i] = binary_op(temp[i], *(first + j));
//         }
//     }

//     // Phase 2: Intermediate Scan
//     _tbb::exclusive_scan(temp.begin(), temp.end(), temp.begin(), *first, binary_op);

//     // Phase 3: Rescan
//     for (size_t i = 0; i < num_tiles; i++)
//     {

//         _tbb::exclusive_scan(first + 1 + i * tile_size,
//                             first + 1 + (i + 1) * tile_size,
//                             d_first + i * tile_size,
//                             temp[i],
//                             binary_op);
//     }
//     if (rem)
//     {
//         _tbb::exclusive_scan(first + num_values - rem + 1,
//                             first + num_values + 1,
//                             d_first + num_values - rem,
//                             temp[num_tiles],
//                             binary_op);
//     }
//     return d_first + num_values;
// }

// template<class InputIt, class OutputIt>
// OutputIt inclusive_scan(InputIt first, InputIt last, OutputIt d_first)
// {
//     return _tbb::tiled::inclusive_scan(first, last, d_first, std::plus<>());
// }

// template<class InputIt> InputIt inclusive_scan(InputIt first, InputIt last)
// {
//     return _tbb::tiled::inclusive_scan(first, last, first, std::plus<>());
// }

// // ----------------------------------------------------------------------------------
// //  Exclusive Scan
// // ----------------------------------------------------------------------------------

// template<class InputIt, class OutputIt, class T, class BinaryOperation>
// OutputIt exclusive_scan(
//     InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
// {
//     using InputType  = typename std::iterator_traits<InputIt>::value_type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<InputType, OutputType>::value,
//                   "Input type must be convertible to output type!");
//     static_assert(std::is_same<InputType, T>::value,
//                   "Underlying input and init type have to be the same!");

//     using InputType  = typename std::iterator_traits<InputIt>::value_type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<InputType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     size_t num_values = last - first;
//     size_t tile_size  = 4;
//     tile_size         = (num_values) > tile_size ? tile_size : 1;
//     size_t num_tiles  = (num_values) / tile_size;
//     size_t rem        = num_values - tile_size * num_tiles;

//     std::vector<InputType> temp(num_tiles + 1);

//     // Phase 1: Reduction
//     for (size_t i = 0; i < num_tiles; i++)
//     {
//         auto range = tbb::blocked_range<int>(first + i *tile_size, first + (i+1) * tile_size);
//         /*temp[i] = tbb::parallel_reduce(range,
//                                         0.0,
//                                         [&](tbb::blocked_range<int> r, InputType running_sum)
//                                         {
//                                             for(int i=r.begin(); i<r.end(); ++i)
//                                             {
//                                                 running_sum += first[i];
//                                             }
//                                             return running_sum;
//                                         },
//                                         std::plus<>());*/
//     }

//     // Phase 2: Intermediate Scan
//     _tbb::exclusive_scan(temp.begin(), temp.end(), temp.begin(), init, binary_op);

//     // Phase 3: Rescan
//     for (size_t i = 0; i < num_tiles; i++)
//     {
//         _tbb::exclusive_scan(first + i * tile_size,
//                             first + (i + 1) * tile_size,
//                             d_first + i * tile_size,
//                             temp[i],
//                             binary_op);
//     }
//     if (rem)
//     {
//         _tbb::exclusive_scan(first + num_values - rem,
//                             first + num_values,
//                             d_first + num_values - rem,
//                             temp[num_tiles],
//                             binary_op);
//     }
//     return d_first + num_values;
// }

// template<class InputIt, class OutputIt, class T>
// OutputIt exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init)
// {
//     return _tbb::tiled::exclusive_scan(first, last, d_first, init, std::plus<>());
// }

// template<class InputIt, class T>
// InputIt exclusive_scan(InputIt first, InputIt last, T init)
// {
//     return _tbb::tiled::exclusive_scan(first, last, first, init, std::plus<>());
// }

// // ----------------------------------------------------------------------------------
// //  Inclusive Segmented Scan
// // ----------------------------------------------------------------------------------

// template<class InputIt, class OutputIt, class BinaryOperation>
// OutputIt inclusive_segmented_scan(InputIt         first,
//                                   InputIt         last,
//                                   OutputIt        d_first,
//                                   BinaryOperation binary_op)
// {
//     using PairType   = typename std::iterator_traits<InputIt>::value_type;
//     using FlagType   = typename std::tuple_element<1, PairType>::type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<FlagType, bool>::value,
//                   "Second Input Iterator type must be convertible to bool!");
//     static_assert(std::is_convertible<PairType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     return _tbb::tiled::inclusive_scan(first,
//                                         last,
//                                         d_first,
//                                         [binary_op](PairType x, PairType y)
//                                         {
//                                             PairType result = y;
//                                             if (!y.second)
//                                             {
//                                                 result.first =
//                                                     binary_op(x.first, y.first);
//                                                 if (x.second)
//                                                 {
//                                                     result.second = x.second;
//                                                 }
//                                             }
//                                             return result;
//                                         });
// }

// template<class InputIt, class OutputIt>
// OutputIt inclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first)
// {
//     return _tbb::tiled::inclusive_segmented_scan(first, last, d_first, std::plus<>());
// }

// template<class InputIt> 
// InputIt inclusive_segmented_scan(InputIt first, InputIt last)
// {
//     return _tbb::tiled::inclusive_segmented_scan(first, last, first, std::plus<>());
// }

// // ----------------------------------------------------------------------------------
// //  Exclusive Segmented Scan
// // ----------------------------------------------------------------------------------

// template<class InputIt, class OutputIt, class BinaryOperation, class T>
// OutputIt exclusive_segmented_scan(
//     InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation binary_op)
// {
//     using PairType   = typename std::iterator_traits<InputIt>::value_type;
//     using FlagType   = typename std::tuple_element<1, PairType>::type;
//     using OutputType = typename std::iterator_traits<OutputIt>::value_type;
//     static_assert(std::is_convertible<FlagType, bool>::value,
//                   "Second Input Iterator type must be convertible to bool!");
//     static_assert(std::is_convertible<PairType, OutputType>::value,
//                   "Input type must be convertible to output type!");

//     _tbb::tiled::exclusive_scan(first,
//                                  last,
//                                  d_first,
//                                  std::make_pair(init, 0),
//                                  [binary_op](PairType x, PairType y)
//                                  {
//                                      PairType result = y;
//                                      if (!y.second)
//                                      {
//                                          result.first = binary_op(x.first, y.first);
//                                          if (x.second)
//                                          {
//                                              result.second = x.second;
//                                          }
//                                      }
//                                      return result;
//                                  });
//     while (first != last)
//     {
//         if (first->second)
//         {
//             d_first->first = init;
//         }
//         first++;
//         d_first++;
//     }
//     return first;
// }

// template<class InputIt, class OutputIt, class T>
// OutputIt exclusive_segmented_scan(InputIt first, InputIt last, OutputIt d_first, T init)
// {
//     return _tbb::tiled::exclusive_segmented_scan(
//         first, last, d_first, init, std::plus<>());
// }

// template<class InputIt, class T>
// InputIt exclusive_segmented_scan(InputIt first, InputIt last, T init)
// {
//     return _tbb::tiled::exclusive_segmented_scan(
//         first, last, first, init, std::plus<>());
// }

// }; // namespace tiled

} // namespace _tbb