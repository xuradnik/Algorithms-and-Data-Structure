/**
 * @file cli_helper_printer.cpp
 * @brief Implementation of @ref aus::Cli_Helper_Printer.
 *
 * Constructor wires up @c procPrint and defines the three menu helpers (`print_help_menu`,
 * `print_filter_flags`, `print_data_meaning`) that build the CLI text UI.
 */

#include "cli_helper_printer.hpp"

namespace aus
{
    Cli_Helper_Printer::Cli_Helper_Printer(std::ostream& ostream) : m_os(ostream)
    {
        procPrint = [this](const Accident& data)
        {
            this->m_os << std::left << std::setw(12) << std::format("{:%d/%m/%Y}", data.date) << " | " << std::setw(6)
                       << std::format("{:%H:%M}", std::chrono::hh_mm_ss{data.time}) << " | " << std::setw(5)
                       << static_cast<int>(data.collision_year) << " | " << std::setw(8)
                       << ("spe:" + std::to_string(static_cast<int>(data.speed_limit))) << " | " << std::setw(7)
                       << ("veh:" + std::to_string(static_cast<int>(data.number_of_vehicles))) << " | " << std::setw(7)
                       << ("cas:" + std::to_string(static_cast<int>(data.number_of_casualties))) << " | "
                       << std::setw(22) << WEATHER.at(static_cast<int>(data.weather_conditions)) << " | "
                       << std::setw(50) << JUNCTION.at(static_cast<int>(data.junction_detail)) << " | " << std::setw(8)
                       << COLLISION_SEVERITY.at(static_cast<int>(data.collision_severity)) << " | " << std::setw(50)
                       << PEDESTRIAN_CROSSING.at(static_cast<int>(data.pedestrian_crossing)) << " | "
                       << DAY_OF_WEEK.at(static_cast<int>(data.day_of_week)) << std::endl;
        };
    }


    void Cli_Helper_Printer::print_help_menu() const
    {
        Terminal::setTextColor(Color::Yellow);
        this->m_os << "\nHELP MENU\n===================================" << std::endl;

        for (uint8_t i = 0; i < this->ARRAY_SIZE_COMMANDS; i++)
        {
            Terminal::setTextColor(Color::Cyan);
            this->m_os << std::left << std::setw(13) << std::string(this->COMMANDS.at(i));

            Terminal::resetTextColor();
            this->m_os << " | " << std::left << std::setw(30) << std::string_view(this->COMMANDS_INFO.at(i)) << " | ";

            Terminal::setTextColor(Color::Yellow);
            this->m_os << std::string_view(this->COMMAND_EXAMPLE.at(i)) << std::endl;
        }
        Terminal::resetTextColor();
    }

    void Cli_Helper_Printer::print_filter_flags() const
    {
        Terminal::setTextColor(Color::Yellow);
        this->m_os << "\nFilter Flags:\n===================================" << std::endl;

        for (uint8_t i = 0; i < ARRAY_SIZE_FLAGS; i++)
        {
            Terminal::setTextColor(Color::Green);
            this->m_os << std::left << std::setw(8) << this->FILTER_FLAGS.at(i);
            Terminal::resetTextColor();

            this->m_os << "| " << std::left << std::setw(39) << this->FILTER_FLAGS_INFO.at(i);
            this->m_os << " |";

            Terminal::setTextColor(Color::Yellow);
            this->m_os << " " << this->FILTER_FLAGS_EXAMPLE.at(i) << std::endl;
        }

        Terminal::resetTextColor();
    }

    void Cli_Helper_Printer::print_data_meaning() const
    {
        Terminal::setTextColor(Color::Yellow);
        this->m_os << "\nData Meaning:\n===================================" << std::endl;

        Terminal::resetTextColor();

        for (const auto& enum_info : ENUM_INFO)
        {
            Terminal::setTextColor(Color::Cyan);
            this->m_os << enum_info.name << std::endl;
            this->m_os << "--------------------" << std::endl;
            Terminal::resetTextColor();

            for (size_t j = 0; j < enum_info.data.size(); j++)
            {
                this->m_os << std::left << std::setw(50) << enum_info.data[j] << " | INDEX " << j << std::endl;
            }
            this->m_os << std::endl;
        }
    }
} // namespace aus
