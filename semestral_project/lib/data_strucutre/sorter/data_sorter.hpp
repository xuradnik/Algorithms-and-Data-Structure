#ifndef AUS_DATA_SORTER_HPP
#define AUS_DATA_SORTER_HPP

/**
 * @file data_sorter.hpp
 * @brief Universal in-place sorter for sequential containers.
 */

#include <functional>
#include <vector>

namespace aus
{

    /**
     * @brief Generic merge-sort wrapper for std::vector<T>.
     * @tparam T Element type. Accident*
     */
    template <typename T>
    class data_sorter
    {
    public:
        /**
         * @brief Sorts @p vec in place with merge sort. O(n log n).
         * @param vec Vector to sort in place.
         * @param compare Returns true when the first argument should come first.
         */
        void sort(
            std::vector<T>& vec,
            const std::function<bool(const T&, const T&)>& compare
            );

    private:
        /**
         * @brief Top-down merge sort over the range [left, right].
         * @param vec Vector being sorted.
         * @param compare Comparator from sort().
         * @param left Inclusive start index.
         * @param right Inclusive end index.
         */
        void merge_sort(
            std::vector<T>& vec,
            const std::function<bool(const T&, const T&)>& compare,
            size_t left,
            size_t right
            );

        /**
         * @brief Merges the two sorted ranges [left, mid] and [mid+1, right].
         *
         * Copies only the T values into a temporary buffer, then writes them
         * back into @p vec.
         *
         * @param vec Vector that holds both ranges.
         * @param compare Comparator from sort().
         * @param left Start of the left range.
         * @param mid End of the left range.
         * @param right End of the right range.
         */
        void merge(
            std::vector<T>& vec,
            const std::function<bool(const T&, const T&)>& compare,
            size_t left,
            size_t mid,
            size_t right
            );
    };


    template <typename T>
    void data_sorter<T>::sort(std::vector<T>& vec, const std::function<bool(const T&, const T&)>& compare)
    {
        if (vec.size() > 1)
            merge_sort(vec, compare, 0, vec.size() - 1);
    }

    template <typename T>
    void data_sorter<T>::merge_sort(
        std::vector<T>& vec, const std::function<bool(const T&, const T&)>& compare, const size_t left, const size_t right)
    {
        if (left >= right)
            return;
        const size_t mid = left + (right - left) / 2;
        merge_sort(vec, compare, left, mid);
        merge_sort(vec, compare, mid + 1, right);
        merge(vec, compare, left, mid, right);
    }

    template <typename T>
    void data_sorter<T>::merge(
        std::vector<T>& vec,
        const std::function<bool(const T&, const T&)>& compare,
        const size_t left, const size_t mid,
        const size_t right
        )
    {
        // helper vector
        std::vector<T> tmp;
        tmp.reserve(right - left + 1);

        size_t i = left;                    // Beginning of the left    vec
        size_t j = mid + 1;                 // Beginning of the right   vec
        while (i <= mid && j <= right)      // I need to go through all elements
        {
            if (compare(vec[i], vec[j]))    // Do I push left or right side first based on sorting
                tmp.push_back(vec[i++]);    // Left side first
            else
                tmp.push_back(vec[j++]);    // Right side first
        }
        while (i <= mid)                    // Check if there are leftover LEFT elemetns and add them to vec
            tmp.push_back(vec[i++]);
        while (j <= right)                  // Check if there are leftover RIGHT elements and add them to vec
            tmp.push_back(vec[j++]);
        for (size_t k = 0; k < tmp.size(); ++k)
            vec[left + k] = tmp[k];         // Replace (reorder) data from the vector
    }

} // namespace aus

#endif // AUS_DATA_SORTER_HPP
