#ifndef AUS_HASH_MAP_DATA_STRUCTURE_HPP
#define AUS_HASH_MAP_DATA_STRUCTURE_HPP

/**
 * @file hash_map_data_structure.hpp
 * @brief Date-indexed hash table for fast accident lookup.
 *
 * Wraps a libds `HashTable<DateKey, AccidentBucket>` keyed on the calendar
 * date. Stores only pointers into the Level-1 records.
 */

#include <chrono>
#include <optional>
#include <string_view>

#include "../../libdf/data_handlers/data_frame.hpp"
#include "../../libds/adt/list.hpp"
#include "../../libds/adt/table.hpp"
#include "../structs/accident.hpp"

namespace aus
{
    /// Per-date list of accident pointers held in an @ref AccidentBucket.
    using AccidentList = ds::adt::ImplicitList<const Accident*>;

    /**
     * @brief Bucket value type stored inside the hash table.
     */
    struct AccidentBucket
    {
        AccidentList accidents;
        bool         operator==(const AccidentBucket&) const { return false; }
        bool         operator!=(const AccidentBucket&) const { return true; }
    };

    /**
     * @brief Hash table keyed by a packed calendar date (YYYYMMDD).
     */
    class Hash_Map_Data_Structure
    {
    public:
        using DateKey = std::uint32_t;

        /** @brief Removes every entry from the table. */
        void clear();

        /**
         * @brief Clears the table, then indexes every record in @p data_frame.
         * @param data_frame Source DataFrame.
         */
        void build(DataFrame<Accident>& data_frame);

        /**
         * @brief Indexes one record under its date key.
         * @param accident Record to index.
         */
        void insert(const Accident& accident);

        /**
         * @brief Looks up the list of pointers for @p date.
         * @param date Calendar date to query.
         * @return Bucket's accident list, or nullptr if there is no match.
         */
        const AccidentList* find(std::chrono::year_month_day date) const;

        /**
         * @brief Parses a DD/MM/YYYY string into a calendar date.
         * @param date Date text to parse.
         * @return Parsed date, or std::nullopt on bad input.
         */
        static std::optional<std::chrono::year_month_day> parse_date(std::string_view date);

        /**
         * @brief Packs a calendar date into the lookup key.
         * @param date Date to encode.
         * @return year*10000 + month*100 + day.
         */
        static DateKey make_date_key(std::chrono::year_month_day date);

        /**
         * @brief std::stoi wrapper that returns std::nullopt on failure.
         * @param text Numeric text to parse.
         * @return Parsed integer, or std::nullopt on error.
         */
        static std::optional<int> parse_int(const std::string_view text);

    private:
        ds::adt::HashTable<DateKey, AccidentBucket> m_date_buckets;
    };

} // namespace aus

#endif // AUS_HASH_MAP_DATA_STRUCTURE_HPP
