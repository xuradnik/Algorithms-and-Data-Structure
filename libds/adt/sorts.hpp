#ifndef LIBDS_ADT_SORTS_HPP
#define LIBDS_ADT_SORTS_HPP

#include <libds/adt/array.hpp>
#include <libds/adt/queue.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <algorithm>
#include <cmath>
#include <functional>
#include <type_traits>


namespace ds::adt {


/**
 * @brief Abstract base for all sorting algorithms operating on an implicit sequence.
 * @tparam T The element type to be sorted; must support operator< for the default comparator.
 */
template<typename T>
struct Sort {
public:
    /**
     * @brief Sorts the sequence supplied comparator.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate; returns true if the first argument should precede the second.
     */
    virtual void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) = 0;

    /**
     * @brief Sorts the sequence in ascending order using the default operator< comparator.
     * @param is The implicit sequence to sort.
     */
    void sort(amt::ImplicitSequence<T> &is) {
        sort(is, [](const T &a, const T &b) -> bool { return a < b; });
    }
};

/**
 * @brief Selection sort: repeatedly finds the minimum element and swaps it into position.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class SelectSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence in-place using the selection sort algorithm.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used to identify the minimum element.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;
};

/**
 * @brief Insertion sort: builds a sorted prefix by shifting each element into its correct position.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class InsertSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence in-place using the insertion sort algorithm.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used to determine insertion position.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;
};

/**
 * @brief Bubble sort: repeatedly passes through the sequence swapping adjacent out-of-order pairs.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class BubbleSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence in-place using the bubble sort algorithm.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate; sorting stops early if no swaps occur.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;
};

/**
 * @brief Quick sort: partitions the sequence around a pivot and recurses on each part.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class QuickSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence in-place using the quicksort algorithm.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used during partitioning.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;

private:
    /**
     * @brief Recursively partitions and sorts the sub-range [min, max] in-place.
     * @param is      The implicit sequence containing the sub-range.
     * @param compare A strict weak ordering predicate used during partitioning.
     * @param min     The inclusive lower index of the sub-range to sort.
     * @param max     The inclusive upper index of the sub-range to sort.
     */
    void quick(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare,
        std::size_t min,
        std::size_t max
    );
};

/**
 * @brief Heap sort: builds an in-place max-heap then repeatedly extracts the maximum element.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class HeapSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence in-place using the heap sort algorithm.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used to maintain the heap invariant.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;
};

/**
 * @brief Shell sort: generalises insertion sort by sorting elements separated by decreasing gaps.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class ShellSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence in-place using the shell sort algorithm with a logarithmic gap sequence.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used during gapped insertion passes.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;

private:
    /**
     * @brief Performs a single pass of gapped insertion sort with gap size @p k, then recurses with k-1.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used during element comparisons.
     * @param k       The current gap size; recursion terminates when k reaches 1.
     */
    void shell(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare,
        std::size_t k
    );
};

/**
 * @brief Radix sort: distributes elements into digit buckets digit-by-digit from least to most significant.
 * @tparam Key An integral type used as the sort key; non-integral keys trigger a static_assert.
 * @tparam T   The element type stored in the sequence.
 */
template<typename Key, typename T>
class RadixSort : public Sort<T> {
    static_assert(
        std::is_integral_v<Key>,
        "Radix sort supports only integral types.");

public:
    /**
     * @brief Constructs a RadixSort that uses the element itself as the key via std::identity.
     */
    RadixSort();

    /**
     * @brief Constructs a RadixSort with a custom key extraction function.
     * @param getKey A callable that extracts an integral key from an element of type T.
     */
    RadixSort(std::function<Key(const T&)> getKey);

    /**
     * @brief Sorts the sequence in-place using LSD radix sort on decimal digits.
     * @param is      The implicit sequence to sort.
     * @param compare Unused; radix sort derives order solely from the extracted integral key.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T&, const T&)> compare
    ) override;

    using Sort<T>::sort;

private:
    /** @brief Callable that extracts the integral sort key from an element. */
    std::function<Key(const T &)> m_getKey; 
};

/**
 * @brief Merge sort: repeatedly splits the sequence into two queues and merges them in sorted order.
 * @tparam T The element type to be sorted.
 */
template<typename T>
class MergeSort : public Sort<T> {
public:
    /**
     * @brief Sorts the sequence using the bottom-up merge sort algorithm.
     * @param is      The implicit sequence to sort.
     * @param compare A strict weak ordering predicate used when merging runs.
     */
    void sort(
        amt::ImplicitSequence<T> &is,
        std::function<bool(const T &, const T &)> compare
    ) override;

    using Sort<T>::sort;

private:
    /**
     * @brief Distributes elements from the merge queue alternately into the two working queues in runs of size @p n.
     * @param n The run length used to determine when to switch between the two queues.
     */
    void split(std::size_t n);

    /**
     * @brief Merges corresponding runs of size @p n from both working queues back into the merge queue.
     * @param compare A strict weak ordering predicate used to select the next element during merging.
     * @param n       The run length determining how many elements are taken from each queue per merge step.
     */
    void merge(
        std::function<bool(const T &, const T &)> compare,
        std::size_t n
    );

private:
    ImplicitQueue<T> *m_queue1 {nullptr};      /**< First working queue holding odd-numbered runs after split. */
    ImplicitQueue<T> *m_queue2 {nullptr};      /**< Second working queue holding even-numbered runs after split. */
    ImplicitQueue<T> *m_mergeQueue {nullptr};  /**< Queue holding the fully merged result after each merge pass. */
};

//----------

template<typename T>
void SelectSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    for (size_t i = 0; i + 1 < is.size(); ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < is.size(); ++j) {
            if (compare(is.access(j)->data, is.access(minIdx)->data)) {
                minIdx = j;
            }
        }
        std::swap(is.access(i)->data, is.access(minIdx)->data);
    }
}

template<typename T>
void InsertSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    for (size_t i = 1; i < is.size(); ++i) {
        T tmp = is.access(i)->data;
        size_t j = i;
        while (j > 0 && compare(tmp, is.access(j - 1)->data)) {
            is.access(j)->data = is.access(j - 1)->data;
            --j;
        }
        is.access(j)->data = tmp;
    }
}

template<typename T>
void BubbleSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    bool swapped;
    do {
        swapped = false;
        for (size_t i = 0; i + 1 < is.size(); ++i) {
            if (compare(is.access(i + 1)->data, is.access(i)->data)) {
                std::swap(is.access(i + 1)->data, is.access(i)->data);
                swapped = true;
            }
        }
    } while (swapped);
}

template<typename T>
void QuickSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    if (!is.isEmpty()) {
        quick(is, compare, 0, is.size() - 1);
    }
}

template<typename T>
void QuickSort<T>::quick(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare,
    std::size_t min,
    std::size_t max
) {
    T pivot = is.access(min + (max - min) / 2)->data;
    size_t left = min;
    size_t right = max;
    do {
        while (compare(is.access(left)->data, pivot)) { ++left; }
        while (right > 0 && compare(pivot, is.access(right)->data)) { --right; }
        if (left <= right) {
            std::swap(is.access(left)->data, is.access(right)->data);
            ++left;
            if (right > 0) { --right; }
        }
    } while (left <= right);
    if (min < right) { quick(is, compare, min, right); }
    if (left < max) { quick(is, compare, left, max); }
}

template<typename T>
void HeapSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    if (is.size() < 2) { return; }
    for (size_t i = 1; i < is.size(); ++i) {
        bool swapped;
        size_t current = i;
        do {
            swapped = false;
            size_t parent = (current - 1) / 2;
            if (current > 0 && compare(is.access(parent)->data, is.access(current)->data)) {
                std::swap(is.access(current)->data, is.access(parent)->data);
                current = parent;
                swapped = true;
            }
        } while (swapped);
    }
    for (size_t i = is.size() - 1; i > 0; --i) {
        std::swap(is.access(0)->data, is.access(i)->data);
        bool swapped;
        size_t current = 0;
        do {
            swapped = false;
            size_t left = 2 * current + 1;
            size_t right = 2 * current + 2;
            size_t maxIdx;
            if (left < i && right < i) {
                maxIdx = compare(is.access(right)->data, is.access(left)->data) ? left : right;
            } else {
                maxIdx = (left < i) ? left : right;
            }
            if (maxIdx < i && compare(is.access(current)->data, is.access(maxIdx)->data)) {
                std::swap(is.access(current)->data, is.access(maxIdx)->data);
                current = maxIdx;
                swapped = true;
            }
        } while (swapped);
    }
}

template<typename T>
void ShellSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    if (is.size() < 2) { return; }
    shell(is, compare, static_cast<size_t>(std::ceil(std::log10(static_cast<double>(is.size())))));
}

template<typename T>
void ShellSort<T>::shell(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare,
    std::size_t k
) {
    for (size_t d = 0; d < k; ++d) {
        for (size_t i = d; i < is.size(); ++i) {
            size_t j = i;
            while (j >= k && j - k >= d && compare(is.access(j)->data, is.access(j - k)->data)) {
                std::swap(is.access(j)->data, is.access(j - k)->data);
                j -= k;
            }
        }
    }
    if (k > 1) { shell(is, compare, k - 1); }
}

template<typename Key, typename T>
RadixSort<Key, T>::RadixSort() :
    m_getKey([](const auto &x) { return x; })
{
}

template<typename Key, typename T>
RadixSort<Key, T>::RadixSort(std::function<Key(const T &)> getKey) :
    m_getKey(getKey) {
}

template<typename Key, typename T>
void RadixSort<Key, T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    (void)compare;
    ExplicitQueue<T>* buckets[10];
    for (int i = 0; i < 10; ++i) { buckets[i] = new ExplicitQueue<T>(); }
    long long component = 1;
    bool hasNext = true;
    while (hasNext) {
        hasNext = false;
        for (size_t idx = 0; idx < is.size(); ++idx) {
            T &elem = is.access(idx)->data;
            Key key = m_getKey(elem);
            buckets[(key / component) % 10]->push(elem);
            if (key / (component * 10) > 0) { hasNext = true; }
        }
        component *= 10;
        size_t index = 0;
        for (int i = 0; i < 10; ++i) {
            while (!buckets[i]->isEmpty()) {
                is.access(index)->data = buckets[i]->peek();
                buckets[i]->pop();
                ++index;
            }
        }
    }
    for (int i = 0; i < 10; ++i) { delete buckets[i]; }
}

template<typename T>
void MergeSort<T>::sort(
    amt::ImplicitSequence<T> &is,
    std::function<bool(const T &, const T &)> compare
) {
    if (is.size() < 2) { return; }
    m_queue1 = new ImplicitQueue<T>(is.size());
    m_queue2 = new ImplicitQueue<T>(is.size());
    m_mergeQueue = new ImplicitQueue<T>(is.size());
    for (size_t i = 0; i < is.size(); ++i) { m_mergeQueue->push(is.access(i)->data); }
    size_t n = 1;
    while (n < is.size()) { split(n); merge(compare, n); n *= 2; }
    split(n); merge(compare, n);
    for (size_t i = 0; i < is.size(); ++i) {
        is.access(i)->data = m_mergeQueue->peek();
        m_mergeQueue->pop();
    }
    delete m_queue1; delete m_queue2; delete m_mergeQueue;
    m_queue1 = nullptr; m_queue2 = nullptr; m_mergeQueue = nullptr;
}

template<typename T>
void MergeSort<T>::split(std::size_t n) {
    size_t count = 0;
    bool isFirst = true;
    while (!m_mergeQueue->isEmpty()) {
        if (count % n == 0) { count = 0; isFirst = !isFirst; }
        if (isFirst) { m_queue1->push(m_mergeQueue->peek()); }
        else         { m_queue2->push(m_mergeQueue->peek()); }
        m_mergeQueue->pop();
        ++count;
    }
}

template<typename T>
void MergeSort<T>::merge(
    std::function<bool(const T &, const T &)> compare,
    std::size_t n
) {
    size_t c1 = 0, c2 = 0;
    do {
        if (c1 == 0 && c2 == 0) {
            c1 = std::min(n, m_queue1->size());
            c2 = std::min(n, m_queue2->size());
        }
        const T *k1 = (c1 > 0 && !m_queue1->isEmpty()) ? &m_queue1->peek() : nullptr;
        const T *k2 = (c2 > 0 && !m_queue2->isEmpty()) ? &m_queue2->peek() : nullptr;
        if (k1 && k2) {
            if (compare(*k1, *k2)) { --c1; m_mergeQueue->push(m_queue1->peek()); m_queue1->pop(); }
            else                   { --c2; m_mergeQueue->push(m_queue2->peek()); m_queue2->pop(); }
        } else if (k1) { --c1; m_mergeQueue->push(m_queue1->peek()); m_queue1->pop(); }
        else if (k2)   { --c2; m_mergeQueue->push(m_queue2->peek()); m_queue2->pop(); }
    } while (!m_queue1->isEmpty() || !m_queue2->isEmpty());
}


} // namespace ds::adt

#endif
