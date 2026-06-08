#ifndef AUS_DATAFRAME_HPP
#define AUS_DATAFRAME_HPP

/**
 * @file data_frame.hpp
 * @brief Generic sequential container with predicate-based iteration.
 *
 * DataFrame<T> stores records in a std::vector and exposes an execute()
 * method for filtering them with a predicate.
 */


#include <functional>
#include <vector>

#include <libds/amt/implicit_sequence.hpp>

namespace aus
{
    /**
     * @brief Generic sequential container with predicate / processor iteration.
     * @tparam T Stored record type Accidents.
     */
    template <typename T>
    class DataFrame
    {
    public:
        /** @brief Builds an empty DataFrame. */
        DataFrame();

        /** @brief Destructor. Clears all stored records. */
        ~DataFrame();

        /**
         * @brief Reserves storage before a batch insert.
         * @param incoming_data_size Expected number of new records.
         */
        void relative_data_size(const std::size_t incoming_data_size);

        /**
         * @brief Appends one record to the container.
         * @param data_to_handle Record to store.
         */
        void handle_incoming_data(T data_to_handle);

        /**
         * @brief Runs @p processor on every record in [begin,end] that matches @p predicate.
         * @tparam Iterator Forward iterator over T.
         * @param begin Range start.
         * @param end Range end.
         * @param predicate Selector callable returning bool.
         * @param processor Callable invoked on matches.
         */
        template <typename Iterator>
        void execute(
            Iterator begin, Iterator end, const std::function<bool(const T&)>& predicate,
            const std::function<void(const T&)>& processor);

        /** @brief Iterator to the first record. */
        std::vector<T>::iterator begin() { return m_data.begin(); }

        /** @brief Past-the-end iterator. */
        std::vector<T>::iterator end() { return m_data.end(); }

    private:
        std::vector<T> m_data;
    };


    template <typename T>
    DataFrame<T>::DataFrame() : m_data()
    {
    }

    template <typename T>
    DataFrame<T>::~DataFrame()
    {
        this->m_data.clear();
    }

    // ======================
    // Relative Data Size
    // ======================
    template <typename T>
    void DataFrame<T>::relative_data_size(const std::size_t incoming_data_size)
    {
        if (this->m_data.size() == 0)
        {
            this->m_data.reserve(incoming_data_size);
            return;
        }
        this->m_data.reserve(this->m_data.size() + incoming_data_size);
    }

    // ======================
    // Handle Incoming Data
    // ======================
    template <typename T>
    void DataFrame<T>::handle_incoming_data(T data_to_handle)
    {
        this->m_data.push_back(data_to_handle);
    }


    // ======================
    // Execute function
    // ======================
    template <typename T>
    template <typename Iterator>
    void DataFrame<T>::execute(
        Iterator begin, Iterator end, const std::function<bool(const T&)>& predicate,
        const std::function<void(const T&)>& processor)
    {
        for (Iterator iterator = begin; iterator != end; ++iterator)
        {
            if (predicate(*iterator))
            {
                processor(*iterator);
            }
        }
    }


} // namespace aus

#endif // AUS_DATAFRAME_HPP
