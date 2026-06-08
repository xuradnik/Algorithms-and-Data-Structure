#ifndef AUS_CLI_COMMANDS_LOGIC_HPP
#define AUS_CLI_COMMANDS_LOGIC_HPP

/**
 * @file cli_commands_logic.hpp
 * @brief logic for filter, load and print commands.
 *
 * Cli_Command_Logic does the actual command work: it parses filter flags,
 * builds and stores a predicate, loads CSV files, then runs the predicate
 * over the DataFrame to print or save matching records.
 */

#include <chrono>
#include <optional>
#include <sstream>
#include <string>

#include "../data_strucutre/sorter/data_sorter.hpp"
#include "../../libdf/data_handlers/csv_loader.hpp"
#include "../../libdf/data_handlers/data_frame.hpp"
#include "../data_strucutre/data_structure.hpp"
#include "../data_strucutre/hash_map_data_structure.hpp"
#include "../structs/accident.hpp"
#include "cli_helper_printer.hpp"

namespace aus
{


    class AccidentPtrIterator
    {
    public:
        using Inner = std::vector<const Accident*>::const_iterator;
        explicit AccidentPtrIterator(const Inner it) : m_it(it) {};
        const Accident&      operator*() const { return **m_it; };
        AccidentPtrIterator& operator++()
        {
            ++m_it;
            return *this;
        };
        bool operator!=(const AccidentPtrIterator& o) const { return m_it != o.m_it; };

    private:
        Inner m_it;
    };
    /**
     * @brief CLI logic.
     */
    class Cli_Command_Logic
    {
    public:
        /**
         * @brief Builds the handler and starts with a "match-all" predicate.
         * @param argc Argument count from main().
         * @param args Argument vector from main().
         * @param data_frame Shared accident DataFrame.
         * @param ostream Output stream for messages.
         * @param printer Shared printer for formatting output.
         */
        Cli_Command_Logic(
            int argc,
            char* args[],
            DataFrame<Accident>& data_frame,
            std::ostream& ostream,
            const Cli_Helper_Printer& printer
            );

        /**
         * @brief filter flags from @p ss and updates the predicate.
         * @param ss Stringstream after the `filter` keyword.
         * @param ignore_missing_args If true, do not error when no flags are given.
         */
        void filter_data_based(std::stringstream& ss, bool ignore_missing_args);

        /**
         * @brief Prints all filtered data.
         * @param ss Stringstream after the `print` keyword.
         */
        void print_all_data_filtered(std::stringstream& ss);

        void print_all_data_N(const int N) const;

        /**
         * @brief Prints all filtered data based date/time.
         * @param ss Stringstream after the `date` keyword.
         */
        void filter_data_based_date(std::stringstream& ss);

        /**
         * @brief Saves matching records into m_filtered_data and prints them.
         * @param ss Stringstream after the `save_data` keyword.
         */
        void save_current_data_to_vector(std::stringstream& ss);

        /**
         * @brief Loads a CSV file into the DataFrame.
         * @param path Filesystem path to the CSV file.
         * @param check If true, skip the file if it was already loaded.
         * @throws std::runtime_error on file or parse errors.
         */
        void load_incoming_data(const std::string& path, const bool check);

        /**
         * @brief Loads file from terminal
         */
        void load_incoming_data_from_arguments();

        /**
         * @brief Prints the stored data in m_filtered_data.
         */
        void print_filtered_data() const;

        /**
         * @brief Runs the active predicate over the DataFrame and prints matches.
         */
        void print_all_data() const;

        void change_doc_state();

        /**
         * @brief Dispatches tree subcommands (up, down, root, ls, print, save_data, sort).
         * @param ss Stringstream after the `tree` keyword.
         */
        void tree_navigate(std::stringstream& ss);

        /**
         * @brief Prints accidents for an exact date via the date hash table.
         * @param ss Stringstream after the command. Expects DD/MM/YYYY.
         */
        void find_accidents_by_date_table(std::stringstream& ss) const;


    private:
        /**
         * @brief Parses a DD/MM/YYYY [HH:MM] [DD/MM/YYYY [HH:MM]] range.
         * @param ss Stream to read from.
         * @param from_date Start date output.
         * @param from_time Start time output (defaults to 00:00).
         * @param to_date End date output (defaults to start date).
         * @param to_time End time output (defaults to 23:59).
         */
        void parse_date_time_range(
            std::stringstream& ss, std::optional<std::chrono::year_month_day>& from_date,
            std::optional<std::chrono::minutes>& from_time, std::optional<std::chrono::year_month_day>& to_date,
            std::optional<std::chrono::minutes>& to_time) const;

        /**
         * @brief Tests every supplied optional condition against @p accident.
         * @param accident Record being tested.
         * @param from_date Optional range start date.
         * @param from_time Optional range start time.
         * @param to_date Optional range end date.
         * @param to_time Optional range end time.
         * @param collision_year Optional collision year.
         * @param speed_limit Optional speed limit.
         * @param min_number_of_vehicles Optional min vehicle count.
         * @param max_number_of_vehicles Optional max vehicle count.
         * @param min_number_of_casualties Optional min casualty count.
         * @param max_number_of_casualties Optional max casualty count.
         * @param weather Optional weather.
         * @param junction Optional junction type.
         * @param collision_severity Optional severity.
         * @param pedestrian_crossing Optional pedestrian crossing type.
         * @param day_of_week Optional day of week.
         * @return True when every supplied condition matches.
         */
        bool predConditions(
            const Accident& accident, const std::optional<std::chrono::year_month_day>& from_date,
            const std::optional<std::chrono::minutes>&        from_time,
            const std::optional<std::chrono::year_month_day>& to_date,
            const std::optional<std::chrono::minutes>& to_time, const std::optional<uint16_t>& collision_year,
            const std::optional<uint16_t>& speed_limit, const std::optional<uint8_t>& min_number_of_vehicles,
            const std::optional<uint8_t>& max_number_of_vehicles,
            const std::optional<uint8_t>& min_number_of_casualties,
            const std::optional<uint8_t>& max_number_of_casualties, const std::optional<Weather>& weather,
            const std::optional<Junction>& junction, const std::optional<Collision_Severity>& collision_severity,
            const std::optional<Pedestrian_Crossing>& pedestrian_crossing,
            const std::optional<Day_Of_Week>&         day_of_week) const;


        /**
         * @brief Prints the current tree position.
         */
        void tree_print_current() const;

        /**
         * @brief Filters under the current tree block, sorts the matches, then prints them.
         * @param ss Stringstream after `sort`. Must start with 1 or 2.
         */
        void sort_from_tree(std::stringstream& ss);


    private:
        int    m_argc;
        char** m_argv;
        bool   m_loaded_data;

        DataFrame<Accident>&                 m_data_frame;
        std::ostream&                        m_os;
        std::vector<const Accident*>         m_filtered_data;
        std::vector<std::string>             m_loaded_data_list;
        std::function<bool(const Accident&)> m_predicate;
        const Cli_Helper_Printer&            m_printer;

        std::optional<Data_Structure> m_tree;
        Data_Structure::Block*        m_current_block{nullptr};
        int                           m_tree_depth{0};
        Hash_Map_Data_Structure       m_date_table;
        data_sorter<const Accident*>  m_sorter;

    private:
        // ===========================
        // Processors
        // ===========================

        /**
         * @brief Processor that appends a record pointer to the filtered vector.
         */
        const std::function<void(std::vector<const Accident*>&, const Accident&)> procInsertInto =
            [](std::vector<const Accident*>& filtered_data, const Accident& accident)
        { filtered_data.push_back(&accident); };

        // ===========================
        // Predicates
        // ===========================

        /** @brief Predicate that accepts every record. */
        const std::function<bool(const Accident&)> predTrue = [](const Accident&) { return true; };

        /**
         * @brief Predicate matching records inside an inclusive date and time window.
         */
        const std::function<bool

                            (const Accident&, const std::chrono::year_month_day&, const std::chrono::minutes&,
                             const std::chrono::year_month_day&, const std::chrono::minutes&


                             )>
            predHappenedBetween = [](const Accident& accident, const std::chrono::year_month_day& from_date,
                                     const std::chrono::minutes& from_time, const std::chrono::year_month_day& to_date,
                                     const std::chrono::minutes& to_time) -> bool
        {
            if (accident.date < from_date || accident.date > to_date)
                return false;
            if (accident.time < from_time || accident.time > to_time)
                return false;
            return true;
        };

        // ===========================
        // Sort Alg
        // ===========================

        /**
         * @brief Order two records by vehicle count, then casualty count.
         */
        const std::function<bool(const Accident*, const Accident*)> compareVehiclesAndCasualties =
            [](const Accident* a, const Accident* b)
        {
            return (
                (a->number_of_vehicles < b->number_of_vehicles) ||
                ((a->number_of_vehicles == b->number_of_vehicles) && (a->number_of_casualties < b->number_of_casualties)));
        };

        /**
         * @brief Order two records chronologically (date, then time).
         */
        const std::function<bool(const Accident*, const Accident*)> compareDateTime =
            [](const Accident* a, const Accident* b)
        { return ((a->date < b->date) || ((a->date == b->date) && (a->time < b->time))); };
    };
} // namespace aus

#endif // AUS_CLI_COMMANDS_LOGIC_HPP
