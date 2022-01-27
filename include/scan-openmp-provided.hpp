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
} // namespace openmp
