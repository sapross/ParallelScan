#pragma once

namespace openmp
{
namespace provided
{
template<typename InputIter, typename OutputIter>
OutputIter inclusive_scan(InputIter first, InputIter last, OutputIter d_first)
{
    using ValueType = typename std::iterator_traits<InputIter>::value_type;

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

template<typename InputIter> InputIter inclusive_scan(InputIter first, InputIter last)
{
    return openmp::provided::inclusive_scan(first, last, first);
}

template<typename InputIter, typename OutputIter, typename T>
OutputIter exclusive_scan(InputIter first, InputIter last, OutputIter d_first, T init)
{
    using ValueType = typename std::iterator_traits<InputIter>::value_type;

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

template<typename InputIter, typename T>
InputIter exclusive_scan(InputIter first, InputIter last, T init)
{
    return openmp::provided::exclusive_scan(first, last, first, init);
}

/*Unfortunately, it is not possible to use the provided function
to implement an inclusive segmented scan*/
template<typename InputIter, typename OutputIter>
OutputIter inclusive_segmented_scan(InputIter first, InputIter last, OutputIter d_first)
{
    using PairType = typename std::iterator_traits<InputIter>::value_type;
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

template<typename InputIter>
InputIter inclusive_segmented_scan(InputIter first, InputIter last)
{
    return openmp::provided::inclusive_segmented_scan(first, last, first);
}

/*Unfortunately, it is not possible to use the provided function
to implement an exclusive segmented scan*/
/*Unfortunately, it is not possible to use the provided function
to implement an exclusive segmented scan*/
template<typename InputIter, typename OutputIter, typename T>
OutputIter
exclusive_segmented_scan(InputIter first, InputIter last, OutputIter d_first, T init)
{
    using PairType  = typename std::iterator_traits<InputIter>::value_type;
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

template<typename InputIter, typename T>
InputIter exclusive_segmented_scan(InputIter first, InputIter last, T init)
{
    return openmp::provided::exclusive_segmented_scan(first, last, first, init);
}
} // namespace provided
} // namespace openmp
