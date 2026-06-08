/**
 * @file data_structure.cpp
 * @brief Date hierarchy implementation from data_structure.hpp.
 *
 * Covers construction, navigation (up/down/root/son_count) and the
 * single-pass loader that turns Accident records into the year/month/day
 * tree described in the header.
 */

#include "data_structure.hpp"

namespace aus
{

    Data_Structure::Data_Structure(DataFrame<Accident>& df) : m_hierarchy_data()
    {
        m_hierarchy_data.emplaceRoot().data.value = 0;
        this->build(df);
    }


    Data_Structure::Block* Data_Structure::access_root() const { return this->m_hierarchy_data.accessRoot(); }

    Data_Structure::Block* Data_Structure::go_up(const Block* current_block) const
    {
        if (current_block == nullptr)
            return nullptr;
        return this->m_hierarchy_data.accessParent(*current_block);
    }

    Data_Structure::Block* Data_Structure::go_down(const Block* current_block, size_t son_index) const
    {
        if (current_block == nullptr)
            return nullptr;
        return this->m_hierarchy_data.accessSon(*current_block, son_index);
    }

    size_t Data_Structure::son_count(const Block* current_block) const
    {
        if (current_block == nullptr)
            return 0;
        return this->m_hierarchy_data.degree(*current_block);
    }

    void Data_Structure::build(DataFrame<Accident>& df) { this->build_range(df.begin(), df.end()); }


    void Data_Structure::additional_data(const Iterator from, const Iterator to) { this->build_range(from, to); }

    Data_Structure::Block* Data_Structure::find_son(const Block& parent, const int value) const
    {
        const size_t count = this->m_hierarchy_data.degree(parent);
        for (size_t i = 0; i < count; i++)
        {
            Block* son = this->m_hierarchy_data.accessSon(parent, i);
            if (son != nullptr && son->data.value == value)
                return son;
        }
        return nullptr;
    }

    void Data_Structure::build_range(const Iterator begin, const Iterator end)
    {
        Block& root                = *this->access_root();
        Block* current_year_block  = nullptr;
        Block* current_month_block = nullptr;
        Block* current_day_block   = nullptr;

        int current_year  = -1;
        int current_month = -1;
        int current_day   = -1;

        for (auto iterator = begin; iterator != end; ++iterator)
        {
            const Accident& accident = *iterator;

            const int year  = static_cast<int>(accident.date.year());
            const int month = static_cast<int>(static_cast<unsigned>(accident.date.month()));
            const int day   = static_cast<int>(static_cast<unsigned>(accident.date.day()));

            if (year != current_year)
            {
                Block* existing    = this->find_son(root, year);
                current_year_block = (existing != nullptr) ? existing : &this->add_son(root, year);
                current_year       = year;
                current_month      = -1;
                current_day        = -1;
            }
            if (month != current_month)
            {
                Block* existing     = this->find_son(*current_year_block, month);
                current_month_block = (existing != nullptr) ? existing : &this->add_son(*current_year_block, month);
                current_month       = month;
                current_day         = -1;
            }
            if (day != current_day)
            {
                Block* existing   = this->find_son(*current_month_block, day);
                current_day_block = (existing != nullptr) ? existing : &this->add_son(*current_month_block, day);
                current_day       = day;
            }

            current_day_block->data.accident_data.insertLast(&accident);
        }
    }


    Data_Structure::Block& Data_Structure::add_son(Block& parent, const int value)
    {
        const std::size_t index  = this->m_hierarchy_data.degree(parent);
        Block&            newSon = this->m_hierarchy_data.emplaceSon(parent, index);
        newSon.data.value        = value;
        return newSon;
    }

} // namespace aus
