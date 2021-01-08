#pragma once

/// <summary>
/// Locale function of [void QuickSort(Iterator, Iterator, Predicator)]. 
/// Divides container elements to 2 parts. In the left - elements which are less then chosen. In the right - that are bigger.
/// </summary>
/// 
/// /// <remarks>
/// Works only if all elements lay near each other in the memory.
/// </remarks>
/// 
/// <typeparam name="Iterator">Iterator of the container</typeparam>
/// <typeparam name="Predicator">Compare function</typeparam>
/// <param name="low">- position of the first element of the container</param>
/// <param name="high">- position of the element, that follows the last element of the container</param>
/// <param name="p">- compare function</param>
/// <returns>Position of the chosen element (pivot)</returns>
template <typename Iterator, typename Predicator>
Iterator _MakingParts(Iterator low, Iterator high, Predicator p)
{
    // element in the middle of the container
    auto pivot = *(low + (high - 1 - low) / 2);

    // algorithm works only with elements of the container (left - first element, right - last element)
    Iterator left = low;
    Iterator right = high - 1;

    // Hoare partition scheme.
    while (left < right) {
        while (p(*left, pivot))
        {
            left++;
        }
        while (p(pivot, *right))
        {
            right--;
        }
        if (left <= right)
        {
            // swap
            auto t = *left;
            *left = *right;
            *right = t;
        }

        // my own addition to code to avoid an infinite loop
        if (!p(*left, *right) && !p(*right, *left))
        {
            left++;
        }
    }
    // right = left after Hoare algorithm
    return right;
}

/// <summary>
/// Function QuickSort() - fast way to sort elements of the container. Check remarks!
/// </summary>
/// <remarks>
/// Works only if all elements lay near each other in the memory.
/// </remarks>
/// <typeparam name="Iterator">Iterator of the container</typeparam>
/// <typeparam name="Predicator">Compare function</typeparam>
/// <param name="begin">- position of the first element of the container</param>
/// <param name="end">- position of the element, that follows the last element of the container</param>
/// <param name="p">- compare function</param>
template <typename Iterator, typename Predicator>
void QuickSort(Iterator begin, Iterator end, Predicator p)
{
    // the border of the recursion (if the length of part is 1)
    if (end == begin)
    {
        return;
    }

    Iterator par = _MakingParts(begin, end, p);

    // recursion. 2 independent QuickSorts for 2 parts of the container.
    if (begin < par)
    {
        QuickSort(begin, par - 1, p);
    }
    if (end > par)
    {
        QuickSort(par + 1, end, p);
    }
}

/// <summary>
/// Function BinarySearch() - quick way to find a position of the most left element, that is more or equal to toFind, or end, if toFind is less, then *begin. Check remarks!
/// </summary>
/// 
/// <remarks>
/// Works only if all elements lay near each other in the memory.
/// Compare function MUST be less (not less or equal)
/// If you need to find position of the element, that is less then toFind, use (return_value - 1); 
/// If toFind is less or more then all elements of an array, then it returns the position of the last element
/// </remarks>
/// 
/// <param name="toFind">- element, position of which we need to find.</param>
/// <param name="begin">- position of the first element of the container</param>
/// <param name="end">- position of the element, that follows the last element of the container</param>
/// <param name="p">- compare function.</param>
/// <returns>Position of an element, that is more or equal to toFind</returns>
template <typename Element, typename Iterator, typename Predicator>
Iterator BinarySearch(Element toFind, Iterator begin, Iterator end, Predicator p)
{

    // function works only with elements from first to last
    end = end - 1;

    // if toFind is less or more then all elements of an array, then it returns the position of the last element
    if (p(toFind, *begin) || p(*end, toFind)) {
        return end;
    }

    // just usual Binary Search algorithm with some modifications
    Iterator left = begin, right = end;
    while ((right - left) > 0 && /* just additional check to avoid confusing situations */ left < end && right > begin)
    {
        Iterator elem = (left + (right - left) / 2);

        // if we found toFind
        if (!p(*elem, toFind) && !p(toFind, *elem))
        {
            return elem;
        }
        // if toFind is in the right part
        if (p(*elem, toFind))
        {
            left = elem + 1;
        }
        // if toFind is in the left part
        else
        {
            right = elem;
        }
    }
    return (right);
}
