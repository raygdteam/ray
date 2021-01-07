
template <typename Iterator, typename Predicator>
Iterator makingParts(Iterator low, Iterator high, Predicator p)
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


template <typename Iterator, typename Predicator>
void QuickSort(Iterator begin, Iterator end, Predicator p)
{
    // the border of the recursion (if the length of part is 1)
    if (end == begin)
    {
        return;
    }

    Iterator par = makingParts(begin, end, p);

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