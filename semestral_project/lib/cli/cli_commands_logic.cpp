/**
 * @file cli_commands_logic.cpp
 * @brief Implementation of @ref aus::Cli_Command_Logic methods.
 *
 */

#include "cli_commands_logic.hpp"
#include <iomanip>
#include <iostream>
#include <iterator>

namespace aus
{



    Cli_Command_Logic::Cli_Command_Logic(
        const int argc, char* args[], DataFrame<Accident>& data_frame, std::ostream& ostream,
        const Cli_Helper_Printer& printer) :
        m_argc(argc), m_argv(args), m_loaded_data(false), m_data_frame(data_frame), m_os(ostream), m_printer(printer)
    {
        this->m_predicate = predTrue;
    }

    void Cli_Command_Logic::filter_data_based(std::stringstream& ss, bool ignore_missing_args)
    {
        std::string argument;
        std::string specification;

        std::optional<std::chrono::year_month_day> from_date;
        std::optional<std::chrono::minutes>        from_time;
        std::optional<std::chrono::year_month_day> to_date;
        std::optional<std::chrono::minutes>        to_time;
        std::optional<uint16_t>                    collision_year;
        std::optional<uint16_t>                    speed_limit;
        std::optional<uint8_t>                     min_number_of_vehicles;
        std::optional<uint8_t>                     max_number_of_vehicles;
        std::optional<uint8_t>                     min_number_of_casualties;
        std::optional<uint8_t>                     max_number_of_casualties;
        std::optional<Weather>                     weather;
        std::optional<Junction>                    junction;
        std::optional<Collision_Severity>          collision_severity;
        std::optional<Pedestrian_Crossing>         pedestrian_crossing;
        std::optional<Day_Of_Week>                 day_of_week;

        bool found_args = false;

        while (ss >> argument)
        {
            found_args = true;

            if (argument == "-T")
            {
                parse_date_time_range(ss, from_date, from_time, to_date, to_time);
            }
            // Collision Year
            else if (argument == "-co")
            {
                if (ss >> specification)
                {
                    try
                    {
                        collision_year = static_cast<uint16_t>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -co" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Speed Limit
            else if (argument == "-l")
            {
                if (ss >> specification)
                {
                    try
                    {
                        speed_limit = static_cast<uint16_t>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -l" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Number Of Vehicles
            else if (argument == "-v")
            {
                std::string first_str;
                if (!(ss >> first_str))
                {
                    this->m_os << "Missing argument for -v (e.g. -v 2  or  -v 2 5  or  -v 2 *  or  -v * 5)"
                               << std::endl;
                }
                else
                {
                    if (first_str != "*")
                    {
                        try
                        {
                            min_number_of_vehicles = static_cast<uint8_t>(std::stoi(first_str));
                        }
                        catch (...)
                        {
                            this->m_os << "Invalid min value for -v" << std::endl;
                        }
                    }

                    const auto  pos = ss.tellg();
                    std::string second_str;
                    if (ss >> second_str && second_str[0] != '-')
                    {
                        if (second_str != "*")
                        {
                            try
                            {
                                max_number_of_vehicles = static_cast<uint8_t>(std::stoi(second_str));
                            }
                            catch (...)
                            {
                                this->m_os << "Invalid max value for -v" << std::endl;
                                max_number_of_vehicles = min_number_of_vehicles;
                                ss.seekg(pos);
                            }
                        }
                    }
                    else
                    {
                        if (ss)
                            ss.seekg(pos);
                        if (first_str != "*")
                        {
                            max_number_of_vehicles = min_number_of_vehicles;
                        }
                    }
                }
            }
            // Number of Casualties
            else if (argument == "-c")
            {
                std::string first_str;
                if (!(ss >> first_str))
                {
                    this->m_os << "Missing argument for -c (e.g. -c 1  or  -c 1 3  or  -c 1 *  or  -c * 3)"
                               << std::endl;
                }
                else
                {
                    if (first_str != "*")
                    {
                        try
                        {
                            min_number_of_casualties = static_cast<uint8_t>(std::stoi(first_str));
                        }
                        catch (...)
                        {
                            this->m_os << "Invalid min value for -c" << std::endl;
                        }
                    }

                    const auto  pos = ss.tellg();
                    std::string second_str;
                    if (ss >> second_str && second_str[0] != '-')
                    {
                        if (second_str != "*")
                        {
                            try
                            {
                                max_number_of_casualties = static_cast<uint8_t>(std::stoi(second_str));
                            }
                            catch (...)
                            {
                                this->m_os << "Invalid max value for -c" << std::endl;
                                max_number_of_casualties = min_number_of_casualties;
                                ss.seekg(pos);
                            }
                        }
                    }
                    else
                    {
                        if (ss)
                            ss.seekg(pos);
                        if (first_str != "*")
                        {
                            max_number_of_casualties = min_number_of_casualties;
                        }
                    }
                }
            }
            // Weather
            else if (argument == "-w")
            {
                if (ss >> specification)
                {
                    try
                    {
                        weather = static_cast<Weather>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -w" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Junction
            else if (argument == "-j")
            {
                if (ss >> specification)
                {
                    try
                    {
                        junction = static_cast<Junction>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -j" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Severity
            else if (argument == "-a")
            {
                if (ss >> specification)
                {
                    try
                    {
                        collision_severity = static_cast<Collision_Severity>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -a" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Pedestrian Crossing
            else if (argument == "-p")
            {
                if (ss >> specification)
                {
                    try
                    {
                        pedestrian_crossing = static_cast<Pedestrian_Crossing>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -p" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Day Of The Week
            else if (argument == "-d")
            {
                if (ss >> specification)
                {
                    try
                    {
                        day_of_week = static_cast<Day_Of_Week>(std::stoi(specification));
                    }
                    catch (...)
                    {
                        this->m_os << "Invalid value for -d" << std::endl;
                    }
                }
                else
                {
                    this->m_os << "Missing argument for " << argument << std::endl;
                }
            }
            // Filter Reset
            else if (argument == "reset" || argument == "-df")
            {
                this->m_predicate = predTrue;
                return;
            }


            else
            {
                this->m_os << "Unknown filter flag: " << argument << std::endl;
            }
        }

        if (!found_args && !ignore_missing_args)
        {
            this->m_os << "Error, filter command needs arguments" << std::endl;
            return;
        }

        this->m_predicate = [this,
                             from_date,
                             from_time,
                             to_date,
                             to_time,
                             collision_year,
                             speed_limit,
                             min_number_of_vehicles,
                             max_number_of_vehicles,
                             min_number_of_casualties,
                             max_number_of_casualties,
                             weather,
                             junction,
                             collision_severity,
                             pedestrian_crossing,
                             day_of_week](const Accident& acc) -> bool
        {
            return this->predConditions(
                acc,
                from_date,
                from_time,
                to_date,
                to_time,
                collision_year,
                speed_limit,
                min_number_of_vehicles,
                max_number_of_vehicles,
                min_number_of_casualties,
                max_number_of_casualties,
                weather,
                junction,
                collision_severity,
                pedestrian_crossing,
                day_of_week);
        };
    }
    void Cli_Command_Logic::save_current_data_to_vector(std::stringstream& ss)
    {
        const auto temp_pred = this->m_predicate;
        this->m_filtered_data.clear();

        this->filter_data_based(ss, true);

        this->m_data_frame.execute(
            this->m_data_frame.begin(),
            this->m_data_frame.end(),
            this->m_predicate,
            [this](const Accident& acc) { this->procInsertInto(this->m_filtered_data, acc); });

        this->m_predicate = temp_pred;
        this->print_filtered_data();
    }
    void Cli_Command_Logic::load_incoming_data(const std::string& path, const bool check)
    {
        const std::filesystem::path load_data(path);

        if (check)
        {
            for (const std::string& s : this->m_loaded_data_list)
            {
                if (path == s)
                {
                    this->m_os << "The file, was already loaded: " << s << std::endl;
                    return;
                }
            }
        }

        CSV_Loader<Accident> csv_loader{load_data, m_data_frame};

        this->m_loaded_data_list.push_back(path);
        this->m_os << "Data Loaded From: " << path << std::endl;
        this->m_loaded_data = true;

        this->m_tree.emplace(this->m_data_frame); // Creation of MultyWayHierarchy inside data_structure (m_tree)
        this->m_current_block = this->m_tree->access_root();
        this->m_tree_depth    = 0;


        this->m_date_table.build(this->m_data_frame);
    }

    void Cli_Command_Logic::load_incoming_data_from_arguments()
    {
        if (this->m_argc <= 1)
        {
            this->m_os << "No arguments for loading data" << std::endl;
            return;
        }

        for (size_t i = 1; i < this->m_argc; i++)
        {
            const std::string path = m_argv[i];
            this->load_incoming_data(path, false);
        }
    }
    void Cli_Command_Logic::print_filtered_data() const
    {
        if (this->m_filtered_data.empty())
        {
            this->m_os << "The Filtered Data List Is Empty" << std::endl;
            return;
        }

        for (const Accident* acc : this->m_filtered_data)
        {
            this->m_printer.procPrint(*acc);
        }
    }
    void Cli_Command_Logic::print_all_data_N(const int N) const
    {
        int i = 0;
        const std::function<bool(const Accident&)> predTrueN =
            [N, &i](const Accident&) {return i++ < N;};

        this->m_data_frame.execute(this->m_data_frame.begin(), this->m_data_frame.end(), predTrueN, this->m_printer.procPrint);
    }
    void Cli_Command_Logic::print_all_data() const
    {
        this->m_data_frame.execute(
            std::make_reverse_iterator(this->m_data_frame.end()), std::make_reverse_iterator( this->m_data_frame.begin()), this->m_predicate, this->m_printer.procPrint);
    }
    void Cli_Command_Logic::change_doc_state()
    {
        auto other = [&](const Accident& acc)
        {
            Weather newWeather = Weather::unknown;

            const_cast<Accident&>(acc).weather_conditions = newWeather;
        };

        this->m_data_frame.execute(this->m_data_frame.begin(), this->m_data_frame.end(), predTrue, other);
        this->m_data_frame.execute(this->m_data_frame.begin(), this->m_data_frame.end(), predTrue, this->m_printer.procPrint);
    }

    void Cli_Command_Logic::print_all_data_filtered(std::stringstream& ss)
    {
        // No inline filter arguments -> keep the active predicate (e.g. set by `date`/`filter`).
        if (ss.rdbuf()->in_avail() == 0 || (ss >> std::ws).eof())
        {
            this->print_all_data();
            return;
        }

        const auto saved = this->m_predicate;
        this->filter_data_based(ss, true);
        this->print_all_data();
        this->m_predicate = saved;
    }
    void Cli_Command_Logic::parse_date_time_range(
        std::stringstream& ss, std::optional<std::chrono::year_month_day>& from_date,
        std::optional<std::chrono::minutes>& from_time, std::optional<std::chrono::year_month_day>& to_date,
        std::optional<std::chrono::minutes>& to_time) const
    {
        auto parse_date = [](const std::string& str) -> std::optional<std::chrono::year_month_day>
        {
            std::istringstream iss(str);
            std::tm            tm = {};
            iss >> std::get_time(&tm, "%d/%m/%Y");
            if (iss.fail())
                return std::nullopt;
            std::chrono::year_month_day ymd = std::chrono::year(tm.tm_year + 1900) / (tm.tm_mon + 1) / tm.tm_mday;
            return ymd;
        };

        auto parse_time = [](const std::string& str) -> std::optional<std::chrono::minutes>
        {
            std::istringstream iss(str);
            std::tm            tm = {};
            iss >> std::get_time(&tm, "%H:%M");
            if (iss.fail())
                return std::nullopt;
            return std::chrono::hours(tm.tm_hour) + std::chrono::minutes(tm.tm_min);
        };

        std::string date1_str, time1_str, date2_str, time2_str;

        if (!(ss >> date1_str))
        {
            this->m_os << "-T command needs at least one date (DD/MM/YYYY) or time (HH:MM) argument" << std::endl;
            return;
        }

        auto date1 = parse_date(date1_str);
        if (!date1)
        {
            auto time1 = parse_time(date1_str);
            if (time1)
            {
                from_date = std::chrono::year::min() / std::chrono::January / 1;
                to_date   = std::chrono::year::max() / std::chrono::December / 31;
                from_time = *time1;
                to_time   = *time1;

                const auto  pos = ss.tellg();
                std::string second_str;
                if (ss >> second_str)
                {
                    if (second_str[0] == '-')
                    {
                        ss.clear();
                        ss.seekg(pos);
                        return;
                    }

                    auto time2 = parse_time(second_str);
                    if (time2)
                        to_time = *time2;
                    else
                    {
                        ss.clear();
                        ss.seekg(pos);
                        this->m_os << "Invalid time format for second time. Expected HH:MM" << std::endl;
                    }
                }
                return;
            }

            this->m_os << "Invalid first argument. Expected date (DD/MM/YYYY) or time (HH:MM)" << std::endl;
            return;
        }

        from_date = *date1;
        to_date   = *date1;
        from_time = std::chrono::minutes(0);
        to_time   = std::chrono::minutes(23 * 60 + 59);

        auto current_pos = ss.tellg();
        if (ss >> time1_str)
        {
            if (time1_str[0] == '-')
            {
                ss.seekg(current_pos);
                return;
            }

            auto time1 = parse_time(time1_str);
            if (time1)
            {
                from_time         = *time1;
                auto current_pos2 = ss.tellg();
                if (ss >> date2_str)
                {
                    if (date2_str[0] == '-')
                    {
                        ss.seekg(current_pos2);
                        to_time = *time1;
                        return;
                    }

                    auto date2 = parse_date(date2_str);
                    if (date2)
                    {
                        to_date           = *date2;
                        auto current_pos3 = ss.tellg();
                        if (ss >> time2_str)
                        {
                            if (time2_str[0] == '-')
                            {
                                ss.seekg(current_pos3);
                                return;
                            }

                            auto time2 = parse_time(time2_str);
                            if (time2)
                                to_time = *time2;

                            else
                                this->m_os << "Invalid time format for second time. Expected HH:MM" << std::endl;
                        }
                    }
                    else
                        this->m_os << "Invalid date format for second date. Expected DD/MM/YYYY" << std::endl;
                }
                else
                    to_time = *time1;
            }
            else
            {
                auto date2 = parse_date(time1_str);
                if (date2)
                {
                    to_date           = *date2;
                    auto current_pos2 = ss.tellg();
                    if (ss >> time2_str)
                    {
                        if (time2_str[0] == '-')
                        {
                            ss.seekg(current_pos2);
                            return;
                        }

                        auto time2 = parse_time(time2_str);
                        if (time2)
                        {
                            from_time         = *time2;
                            auto current_pos3 = ss.tellg();
                            if (ss >> time1_str)
                            {
                                if (time1_str[0] == '-')
                                {
                                    ss.seekg(current_pos3);
                                    to_time = *time2;
                                    return;
                                }

                                auto real_time2 = parse_time(time1_str);
                                if (real_time2)
                                    to_time = *real_time2;

                                else
                                    this->m_os << "Invalid time format for second time. Expected HH:MM" << std::endl;
                            }
                            else
                                to_time = *time2;
                        }
                        else
                            this->m_os << "Invalid time format. Expected HH:MM" << std::endl;
                    }
                }
                else
                    this->m_os << "Invalid argument format. Expected time (HH:MM) or date (DD/MM/YYYY)" << std::endl;
            }
        }
    }
    void Cli_Command_Logic::filter_data_based_date(std::stringstream& ss)
    {
        std::optional<std::chrono::year_month_day> from_date;
        std::optional<std::chrono::minutes>        from_time;
        std::optional<std::chrono::year_month_day> to_date;
        std::optional<std::chrono::minutes>        to_time;

        parse_date_time_range(ss, from_date, from_time, to_date, to_time);

        if (from_date.has_value())
        {
            this->m_predicate = [this, from_date, from_time, to_date, to_time](const Accident& acc) -> bool
            {
                return this->predHappenedBetween(
                    acc, from_date.value(), from_time.value(), to_date.value(), to_time.value());
            };
        }
    }

    bool Cli_Command_Logic::predConditions(
        const Accident& accident, const std::optional<std::chrono::year_month_day>& from_date,
        const std::optional<std::chrono::minutes>& from_time, const std::optional<std::chrono::year_month_day>& to_date,
        const std::optional<std::chrono::minutes>& to_time, const std::optional<uint16_t>& collision_year,
        const std::optional<uint16_t>& speed_limit, const std::optional<uint8_t>& min_number_of_vehicles,
        const std::optional<uint8_t>& max_number_of_vehicles, const std::optional<uint8_t>& min_number_of_casualties,
        const std::optional<uint8_t>& max_number_of_casualties, const std::optional<Weather>& weather,
        const std::optional<Junction>& junction, const std::optional<Collision_Severity>& collision_severity,
        const std::optional<Pedestrian_Crossing>& pedestrian_crossing,
        const std::optional<Day_Of_Week>&         day_of_week) const
    {
        if (from_date.has_value() && to_date.has_value() && from_time.has_value() && to_time.has_value())
        {
            if (!this->predHappenedBetween(
                    accident, from_date.value(), from_time.value(), to_date.value(), to_time.value()))
                return false;
        }

        if (collision_year.has_value() && accident.collision_year != collision_year.value())
            return false;
        if (speed_limit.has_value() && accident.speed_limit != speed_limit.value())
            return false;
        if (min_number_of_vehicles.has_value() && accident.number_of_vehicles < min_number_of_vehicles.value())
            return false;
        if (max_number_of_vehicles.has_value() && accident.number_of_vehicles > max_number_of_vehicles.value())
            return false;
        if (min_number_of_casualties.has_value() && accident.number_of_casualties < min_number_of_casualties.value())
            return false;
        if (max_number_of_casualties.has_value() && accident.number_of_casualties > max_number_of_casualties.value())
            return false;
        if (weather.has_value() && accident.weather_conditions != weather.value())
            return false;
        if (junction.has_value() && accident.junction_detail != junction.value())
            return false;
        if (collision_severity.has_value() && accident.collision_severity != collision_severity.value())
            return false;
        if (pedestrian_crossing.has_value() && accident.pedestrian_crossing != pedestrian_crossing.value())
            return false;
        if (day_of_week.has_value() && accident.day_of_week != day_of_week.value())
            return false;

        return true;
    }

    // TABLE HASH
    void Cli_Command_Logic::find_accidents_by_date_table(std::stringstream& ss) const
    {
        if (!this->m_loaded_data)
        {
            this->m_os << "No data loaded. Use 'load <path>' first." << std::endl;
            return;
        }

        std::string date_text;
        if (!(ss >> date_text))
        {
            this->m_os << "Usage: date_table DD/MM/YYYY" << std::endl;
            return;
        }

        const auto parsed_date = Hash_Map_Data_Structure::parse_date(date_text);
        if (!parsed_date.has_value())
        {
            this->m_os << "Invalid date format. Expected DD/MM/YYYY" << std::endl;
            return;
        }

        const auto* accidents = this->m_date_table.find(parsed_date.value());
        if (accidents == nullptr || accidents->isEmpty())
        {
            this->m_os << "No accidents found for date: " << date_text << std::endl;
            return;
        }

        for (const Accident* accident : *accidents)
        {
            this->m_printer.procPrint(*accident);
        }
    }
    void Cli_Command_Logic::tree_navigate(std::stringstream& ss)
    {
        constexpr int AT_ROOT = 0;

        if (!this->m_tree.has_value())
        {
            this->m_os << "No data loaded. Use 'load path' first." << std::endl;
            return;
        }

        std::string command;
        if (!(ss >> command))
        {
            this->tree_print_current();
            return;
        }

        if (command == "up")
        {
            if (this->m_tree_depth == AT_ROOT)
                this->m_os << "Already at root" << std::endl;
            else
            {
                this->m_current_block = this->m_tree->go_up(this->m_current_block);
                --this->m_tree_depth;
                this->tree_print_current();
            }
        }
        else if (command == "down")
        {
            size_t parameter_of_the_command;
            if (!(ss >> parameter_of_the_command))
            {
                this->m_os << "Usage: tree down <index>" << std::endl;
                return;
            }
            const size_t son_count = this->m_tree->son_count(this->m_current_block);
            if (parameter_of_the_command >= son_count)
            {
                this->m_os << "Index out of range. Children: " << son_count << std::endl;
                return;
            }

            this->m_current_block = this->m_tree->go_down(this->m_current_block, parameter_of_the_command);
            ++this->m_tree_depth;
            this->tree_print_current();
        }
        else if (command == "root")
        {
            this->m_current_block = this->m_tree->access_root();
            this->m_tree_depth    = 0;
            this->tree_print_current();
        }
        else if (command == "ls" || command == "list")
        {
            const size_t son_count = this->m_tree->son_count(this->m_current_block);
            if (son_count == 0)
                this->m_os << "No children." << std::endl;

            else
            {
                constexpr const char* child_label[] = {"Year", "Month", "Day", "?"};
                constexpr const char* month_names[] = {
                    "",
                    "January",
                    "February",
                    "March",
                    "April",
                    "May",
                    "June",
                    "July",
                    "August",
                    "September",
                    "October",
                    "November",
                    "December"};
                int cl = (this->m_tree_depth < 3) ? this->m_tree_depth + 1 : 3;
                for (size_t i = 0; i < son_count; ++i)
                {
                    auto* child = this->m_tree->go_down(this->m_current_block, i);
                    int   val   = child->data.value;
                    this->m_os << "[" << i << "] " << child_label[cl - 1] << ": ";
                    if (cl == 2 && val >= 1 && val <= 12)
                        this->m_os << month_names[val];
                    else
                        this->m_os << val;
                    if (cl == 3)
                        this->m_os << "  (" << child->data.accident_data.size() << " accidents)";
                    else
                        this->m_os << "  (" << this->m_tree->son_count(child) << " children)";
                    this->m_os << "\n";
                }
            }
        }
        else if (command == "print")
        {
            auto saved_predicate = this->m_predicate;
            this->filter_data_based(ss, true);
            this->m_data_frame.execute(
                this->m_tree->hier_begin(this->m_current_block),
                this->m_tree->hier_end(),
                this->m_predicate,
                this->m_printer.procPrint);
            this->m_predicate = saved_predicate;
        }
        else if (command == "save_data")
        {
            auto saved_predicate = this->m_predicate;
            this->filter_data_based(ss, true);
            this->m_filtered_data.clear();
            this->m_data_frame.execute(
                this->m_tree->hier_begin(this->m_current_block),
                this->m_tree->hier_end(),
                this->m_predicate,
                [this](const Accident& acc) { this->procInsertInto(this->m_filtered_data, acc); });
            this->print_filtered_data();
            this->m_predicate = saved_predicate;
        }
        else if (command == "sort")
        {
            this->sort_from_tree(ss);
        }
        else if (command == "filter")
        {
            const auto  pos = ss.tellg();
            std::string command;
            if (!(ss >> command))
            {
                this->m_os << "Usage: tree filter <flags>   (same flags as global 'filter', "
                              "e.g. -T 12/03/2023, -co 2024, -w 1, -a 0)"
                           << std::endl;
                return;
            }
            ss.clear();
            ss.seekg(pos);

            this->filter_data_based(ss, false);
            this->m_data_frame.execute(
                this->m_tree->hier_begin(this->m_current_block),
                this->m_tree->hier_end(),
                this->m_predicate,
                this->m_printer.procPrint);
        }
        else
        {
            this->m_os << "Unknown subcommand. Available: up | down <n> | root | ls | print | filter | save_data | "
                          "sort <1|2> [flags] [save_data]"
                       << std::endl;
        }

    }
    void Cli_Command_Logic::tree_print_current() const
    {
        if (!this->m_current_block)
            return;

        constexpr const char* labels[] = {"Root", "Year", "Month", "Day"};
        int                   val      = this->m_current_block->data.value;

        this->m_os << "[ " << labels[this->m_tree_depth] << " ] ";
        if (this->m_tree_depth == 0)
            this->m_os << "(root)";
        else
            this->m_os << val;

        if (this->m_tree_depth == 3)
            this->m_os << " — " << this->m_current_block->data.accident_data.size() << " accidents";
        else
            this->m_os << " — " << this->m_tree->son_count(this->m_current_block) << " children";

        this->m_os << "\n";
    }

    //  Sort
    void Cli_Command_Logic::sort_from_tree(std::stringstream& ss)
    {
        if (!m_tree.has_value())
        {
            m_os << "No data loaded. Use 'load <path> first'" << std::endl;
            return;
        }

        bool                     save_result = false;
        std::vector<std::string> tokens;
        std::string              token;
        while (ss >> token)
        {
            if (token == "save_data")
                save_result = true;
            else
                tokens.push_back(token);
        }

        int choice = 0;
        if (!tokens.empty())
        {
            try
            {
                choice = std::stoi(tokens.front());
            }
            catch (...)
            {
                choice = 0;
            }
        }
        if (choice != 1 && choice != 2)
        {
            m_os << "Usage: tree sort <1|2> [filter flags] [save_data]\n"
                 << "  1 = compareDateTime\n"
                 << "  2 = compareVehiclesAndCasualties\n" <<  std::endl;
            return;
        }


        std::stringstream filter_stream;
        for (size_t i = 1; i < tokens.size(); ++i)
            filter_stream << tokens[i] << ' ';

        const auto saved_predicate = m_predicate;
        filter_data_based(filter_stream, true);

        std::vector<const Accident*> temp;
        m_data_frame.execute(
            m_tree->hier_begin(m_current_block),
            m_tree->hier_end(),
            m_predicate,
            [&temp](const Accident& acc) { temp.push_back(&acc); });

        m_predicate = saved_predicate;

        if (temp.empty())
        {
            m_os << "No matching accidents at current position." << std::endl;
            return;
        }

        if (choice == 1)
            m_sorter.sort(temp, compareDateTime);
        else
            m_sorter.sort(temp, compareVehiclesAndCasualties);

        std::function<void(const Accident&)> printWithKeys;
        if (choice == 1)
        {
            printWithKeys = [this](const Accident& acc)
            {
                const auto d    = acc.date;
                const auto mins = acc.time.count();
                m_os << "[" << static_cast<int>(d.year()) << "/" << std::setw(2) << std::setfill('0') << static_cast<unsigned>(d.month()) << "/"
                     << std::setw(2) << std::setfill('0') << static_cast<unsigned>(d.day()) << " " << std::setw(2)
                     << std::setfill('0') << mins / 60 << ":" << std::setw(2) << std::setfill('0') << mins % 60 << "] "
                     << std::setfill(' ');
                m_printer.procPrint(acc);
            };
        }
        else
        {
            printWithKeys = [this](const Accident& acc)
            {
                m_os << "[V:" << static_cast<int>(acc.number_of_vehicles) << " C:" << static_cast<int>(acc.number_of_casualties) << "] ";
                m_printer.procPrint(acc);
            };
        }

        m_data_frame.execute
        (
            AccidentPtrIterator(temp.cbegin()),
            AccidentPtrIterator(temp.cend()),
            predTrue,
            printWithKeys
            );

        if (save_result)
            m_filtered_data = temp;

    }


} // namespace aus
