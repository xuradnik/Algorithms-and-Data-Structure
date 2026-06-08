#ifndef AUS_CLI_HELPER_PRINTER_HPP
#define AUS_CLI_HELPER_PRINTER_HPP

/**
 * @file cli_helper_printer.hpp
 * @brief Formatted output helpers for menus and accident records.
 *
 * Cli_Helper_Printer keeps every user-visible string in one place:
 * command help, filter-flag descriptions, enum mappings, and the
 * per-record print formatter exposed through @c procPrint.
 */

#include <array>
#include <functional>
#include <iomanip>
#include <span>
#include <string_view>

#include "../../libdf/terminal.hpp"
#include "../structs/accident.hpp"

namespace aus
{
    /**
     * @brief Formatted printer for the CLI.
     */
    class Cli_Helper_Printer
    {
    public:
        /**
         * @brief Builds the printer and wires up @ref procPrint.
         * @param ostream Output stream for all text.
         */
        Cli_Helper_Printer(std::ostream& ostream);

        /** @brief Prints the available commands. */
        void print_help_menu() const;

        /** @brief Prints every filter flag and its expected arguments. */
        void print_filter_flags() const;

        /** @brief Prints the number-to-name mapping for each enum field. */
        void print_data_meaning() const;

        /**
         * @brief Prints Accident.
         */
        std::function<void(const Accident&)> procPrint;


    private:
        std::ostream& m_os;


        static constexpr uint8_t ARRAY_SIZE_COMMANDS = 13;

        static constexpr std::array<std::string_view, ARRAY_SIZE_COMMANDS> COMMANDS = {
            "help",
            "flags",
            "data_meaning",
            "exit",
            "load",
            "print",
            "print_N",
            "print -d",
            "filter",
            "date",
            "date_table",
            "save_data",
            "tree"};
        static constexpr std::array<std::string_view, ARRAY_SIZE_COMMANDS> COMMANDS_INFO = {
            "print basic commands",
            "print filter flags",
            "print data meaning",
            "exit the program",
            "load CSV file",
            "print filtered records",
            "print data to N-th range",
            "print saved snapshot",
            "set filter by flags",
            "set date/time range filter",
            "find accidents by exact date",
            "save filtered data to list",
            "navigate date hierarchy"};
        static constexpr std::array<std::string_view, ARRAY_SIZE_COMMANDS> COMMAND_EXAMPLE = {
            "",
            "",
            "",
            "",
            "load example_file.txt",
            "",
            "print_N 100",
            "",
            "",
            "",
            "date_table 12/10/2023",
            "save_data -T 12/03/2023 -w 1 -a 1 -j 3",
            "tree ls | tree down 0 | tree up | tree root | tree print | tree filter -T 7:00 8:00 | tree sort 1 | tree sort 2"};

        static constexpr uint8_t ARRAY_SIZE_FLAGS = 12;

        static constexpr std::array<std::string_view, ARRAY_SIZE_FLAGS> FILTER_FLAGS = {
            "-co", "-l", "-v", "-c", "-w", "-j", "-a", "-p", "-d", "reset", "-df", "-T"};
        static constexpr std::array<std::string_view, ARRAY_SIZE_FLAGS> FILTER_FLAGS_INFO = {
            "Filter Accident By Collision Year",
            "Filter Accident By Speed Limit",
            "Filter Accident By Number Of Vehicles",
            "Filter Accident By Number Of Casualties",
            "Filter Accident By Weather",
            "Filter Accident By Junction",
            "Filter Accident Severity",
            "Filter Accident By Pedestrian Crossing",
            "Filter Accident By Day Of The Week",
            "Filter Reset",
            "Filter Reset",
            "Filter Accident By Date/Time Range"};
        static constexpr std::array<std::string_view, ARRAY_SIZE_FLAGS> FILTER_FLAGS_EXAMPLE = {
            "-co 1  (  0 to 255 )",
            "-l 30  ( 0 to 255 )",
            "-v MIN [MAX]  ( e.g. -v 2 : -v 2 5 : -v 2 * : -v * 5 )",
            "-c MIN [MAX]  ( e.g. -c 1 : -c 1 3 : -c 1 * : -c * 3 )",
            "-w 1   ( -1 : 1 : 2 : 3 : 4 : 5 : 6 : 7 : 8 : 9  )",
            "-j 0   ( 0 : 1 : 2 : 3 : 4 : 5 : 6  )",
            "-a 1   (  1 : 2 : 3 : 4  )",
            "-p 0   ( -1 : 0 : 11 : 12 : 13 : 14 : 15 : 16 : 17 : 99  )",
            "-d 1   ( -1 : 1 : 2 : 3 : 4 : 5 : 6 : 7  )",
            "",
            "",
            "-T 7:00 8:00 (time only) | -T 15/05/2020 16:00 20/05/2020 20:00"};


        static constexpr std::array<std::string_view, 10> WEATHER = {
            "Unmarked Data",
            "Fine No High Winds",
            "Raining No High Winds",
            "Snowing No High Winds",
            "Fine High Winds",
            "Raining High Winds",
            "Snowing High Winds",
            "Fog Or Mist",
            "Other",
            "Unknown"};
        static constexpr std::array<std::string_view, 7> JUNCTION = {
            "Data Missing Or Out Of Range",
            "Not At Junction Or Within 20 Meters",
            "T Or Staggered Junction",
            "crossroads",
            "Junction With More Than Four Arms Not Roundabout",
            "Private Drive Or Entrance",
            "Unknown"};
        static constexpr std::array<std::string_view, 4> COLLISION_SEVERITY = {
            "killed", "serious", "slight", "unknown"};
        static constexpr std::array<std::string_view, 10> PEDESTRIAN_CROSSING = {
            "Data Missing Or Out Of Range",
            "No Physical Crossing Facility Within 50m",
            "Human Crossing Control By School Crossing Patrol",
            "Human Crossing Control By Other Authorised Person",
            "Zebra Crossing",
            "Pedestrian Light Crossing",
            "Pedestrian Phase At Traffic Signal",
            "Footbridge Or Subway",
            "Central Refuge",
            "Unknown"};
        static constexpr std::array<std::string_view, 8> DAY_OF_WEEK = {
            "Data Missing Or Out Of Range",
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"};

        /** @brief Pairs an enum's display name with its label table. */
        struct EnumDataInfo
        {
            std::string_view                  name;
            std::span<const std::string_view> data;
        };

        static constexpr std::array<EnumDataInfo, 5> ENUM_INFO = {
            {{"WEATHER | -w", WEATHER},
             {"JUNCTION | -j", JUNCTION},
             {"COLLISION SEVERITY | -a", COLLISION_SEVERITY},
             {"PEDESTRIAN CROSSING | -p", PEDESTRIAN_CROSSING},
             {"DAY OF WEEK | -d", DAY_OF_WEEK}}};
    };
} // namespace aus

#endif // AUS_CLI_HELPER_PRINTER_HPP
