/**
 * @file hash_map_data_structure.cpp
 * @brief Implementation of @ref aus::Hash_Map_Data_Structure.
 *
 * Holds the bucket-append insert path and the date-parsing helpers
 * declared in hash_map_data_structure.hpp.
 */

#include "hash_map_data_structure.hpp"
#include <string>

namespace aus
{
    void Hash_Map_Data_Structure::clear() { this->m_date_buckets.clear(); }

    void Hash_Map_Data_Structure::build(DataFrame<Accident>& data_frame)
    {
        this->clear();
        for (auto& it : data_frame)
        {
            this->insert(it);
        }
    }

    void Hash_Map_Data_Structure::insert(const Accident& accident)
    {
        const DateKey   key    = make_date_key(accident.date);
        AccidentBucket* bucket = nullptr;
        if (this->m_date_buckets.tryFind(key, bucket))
        {
            bucket->accidents.insertLast(&accident);
        }
        else
        {
            AccidentBucket new_bucket;
            new_bucket.accidents.insertLast(&accident);
            this->m_date_buckets.insert(key, new_bucket);
        }
    }

    const AccidentList* Hash_Map_Data_Structure::find(const std::chrono::year_month_day date) const
    {
        const DateKey   key    = make_date_key(date);
        AccidentBucket* bucket = nullptr;
        if (this->m_date_buckets.tryFind(key, bucket))
        {
            return &bucket->accidents;
        }
        return nullptr;
    }

    std::optional<std::chrono::year_month_day> Hash_Map_Data_Structure::parse_date(std::string_view date)
    {
        const size_t first_slash = date.find('/');
        if (first_slash == std::string_view::npos)
            return std::nullopt;


        const size_t second_slash = date.find('/', first_slash + 1);
        if (second_slash == std::string_view::npos)
            return std::nullopt;


        const std::optional<int> day   = parse_int(date.substr(0, first_slash));
        const std::optional<int> month = parse_int(date.substr(first_slash + 1, second_slash - first_slash - 1));
        const std::optional<int> year  = parse_int(date.substr(second_slash + 1));

        if (!day.has_value() || !month.has_value() || !year.has_value())
            return std::nullopt;


        const std::chrono::year_month_day parsed_date = std::chrono::year(*year) /
            std::chrono::month(static_cast<unsigned>(*month)) / std::chrono::day(static_cast<unsigned>(*day));

        if (!parsed_date.ok())
            return std::nullopt;


        return parsed_date;
    }

    Hash_Map_Data_Structure::DateKey Hash_Map_Data_Structure::make_date_key(const std::chrono::year_month_day date)
    {
        const auto year  = static_cast<int>(date.year());
        const auto month = static_cast<unsigned>(date.month());
        const auto day   = static_cast<unsigned>(date.day());

        return static_cast<DateKey>(year * 10000 + static_cast<int>(month) * 100 + static_cast<int>(day));
    }

    std::optional<int> Hash_Map_Data_Structure::parse_int(const std::string_view text)
    {
        try
        {
            return std::stoi(std::string(text));
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
} // namespace aus
