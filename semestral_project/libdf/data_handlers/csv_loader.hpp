#ifndef AUS_CSV_LOADER_HPP
#define AUS_CSV_LOADER_HPP

/**
 * @file csv_loader.hpp
 * @brief CSV parser that fills a DataFrame with accident records.
 *
 * Reads a comma-separated file. The first row is the header (column
 * names map to indices). The rest are parsed into Accident records and
 * appended to the DataFrame.
 */

#include "../../lib/structs/accident.hpp"
#include "data_frame.hpp"

#include <libds/adt/table.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>


namespace aus
{
    /**
     * @brief Parses a CSV file into a DataFrame<T> of Accident records.
     * @tparam T Record type Accident.
     *
     * Parsing happens in the constructor. The first row must hold the
     * expected DfT column headers.
     */
    template <typename T>
    class CSV_Loader
    {
    public:
        /**
         * @brief Opens the file, parses it, and fills the DataFrame.
         * @param path_to_file Path to the CSV file.
         * @param dataframe DataFrame to fill.
         * @throws std::runtime_error if the file cannot be opened or parsed.
         */
        CSV_Loader(const std::filesystem::path& path_to_file, DataFrame<T>& dataframe);

    private:
        /**
         * @brief Main parsing loop. Reads the header, then walks each data row.
         *
         * @param path_to_file Path forwarded from the constructor.
         */
        void load_csv_data(const std::filesystem::path& path_to_file);

        /**
         * @brief Parses a DD/MM/YYYY cell into @p data.date.
         * @param data                       Accident to populate.
         * @param date_from_separated_cells  Raw text of the date cell.
         * @throws std::runtime_error on bad format.
         */
        void handle_date(Accident& data, std::string_view date_from_separated_cells);

        /**
         * @brief Parses an HH:MM cell into @p data.time.
         * @param data                       Accident to populate.
         * @param time_from_separated_cells  Raw text of the time cell.
         * @throws std::runtime_error on bad format.
         */
        void handle_time(Accident& data, std::string_view time_from_separated_cells);

        /**
         * @brief Translates the weather code into a Weather enum value.
         * @param data                            Accident to populate.
         * @param weather_from_separated_cells    Raw text of the weather cell.
         * @throws std::runtime_error on unknown codes.
         */
        void handle_weather_conditions(Accident& data, std::string_view weather_from_separated_cells);

        /**
         * @brief Translates the junction code into a Junction enum value.
         * @param data                             Accident to populate.
         * @param junction_from_separated_cells    Raw text of the junction cell.
         * @throws std::runtime_error on unknown codes.
         */
        void handle_junction_details(Accident& data, std::string_view junction_from_separated_cells);

        /**
         * @brief Translates the severity code into a Collision_Severity value.
         * @param data                              Accident to populate.
         * @param collision_from_separated_cells    Raw text of the severity cell.
         * @throws std::runtime_error on unknown codes.
         */
        void handle_collision_severity(Accident& data, std::string_view collision_from_separated_cells);

        /**
         * @brief Translates the pedestrian-crossing code into the matching enum.
         * @param data                               Accident to populate.
         * @param pedestrian_from_separated_cells    Raw text of the crossing cell.
         * @throws std::runtime_error on unknown codes.
         */
        void handle_pedestrian_crossing(Accident& data, std::string_view pedestrian_from_separated_cells);

        /**
         * @brief Translates the day-of-week code into a Day_Of_Week value.
         * @param data                                Accident to populate.
         * @param day_of_week_from_separated_cells    Raw text of the day cell.
         * @throws std::runtime_error on unknown codes.
         */
        void handle_day_of_week(Accident& data, std::string_view day_of_week_from_separated_cells);

        DataFrame<T>& m_data_frame;
    };



    template <typename T>
    CSV_Loader<T>::CSV_Loader(const std::filesystem::path& path_to_file, DataFrame<T>& dataframe) :
        m_data_frame(dataframe)
    {
        this->load_csv_data(path_to_file);
    }


    // ======================
    // Load CSV Data Function
    // ======================
    template <typename T>
    void CSV_Loader<T>::load_csv_data(const std::filesystem::path& path_to_file)
    {
        std::ifstream file(path_to_file);
        if (!file.is_open())
            throw std::runtime_error("Failed to open a file: '" + path_to_file.string() + "'");


        ds::adt::HashTable<std::string, int> mapa;
        mapa.insert("date", -1);
        mapa.insert("time", -1);
        mapa.insert("collision_year", -1);
        mapa.insert("number_of_vehicles", -1);
        mapa.insert("number_of_casualties", -1);
        mapa.insert("weather_conditions", -1);
        mapa.insert("junction_detail", -1);
        mapa.insert("collision_severity", -1);
        mapa.insert("speed_limit", -1);
        mapa.insert("pedestrian_crossing", -1);
        mapa.insert("day_of_week", -1);

        constexpr int DEFAULT_EXPECTED_FILE_SIZE = 10000;
        this->m_data_frame.relative_data_size(DEFAULT_EXPECTED_FILE_SIZE);

        std::string csv_line;
        if (std::getline(file, csv_line))
        {
            std::stringstream ss(csv_line);
            std::string       column_name;
            int               column_index = 0;

            while (std::getline(ss, column_name, ','))
            {
                int* mapped_index = nullptr;
                if (mapa.tryFind(column_name, mapped_index))
                {
                    *mapped_index = column_index;
                }
                column_index++;
            }
        }

        for (const ds::adt::TableItem<std::string, int>& column : mapa)
        {
            if (column.data == -1)
                throw std::runtime_error("Invalid data. The '" + column.key + "' was not found.");
        }


        const uint_fast64_t date_index = mapa.find("date"), time_index = mapa.find("time"),
                            collision_year_index       = mapa.find("collision_year"),
                            number_of_vehicles_index   = mapa.find("number_of_vehicles"),
                            number_of_casualties_index = mapa.find("number_of_casualties"),
                            weather_conditions_index   = mapa.find("weather_conditions"),
                            junction_detail_index      = mapa.find("junction_detail"),
                            collision_severity_index   = mapa.find("collision_severity"),
                            speed_limit_index          = mapa.find("speed_limit"),
                            pedestrian_crossing_index  = mapa.find("pedestrian_crossing"),
                            day_of_week_index          = mapa.find("day_of_week");


        std::vector<std::string_view> separated_cells_from_line;
        separated_cells_from_line.reserve(32);

        while (std::getline(file, csv_line))
        {
            separated_cells_from_line.clear();

            std::string_view line_view(csv_line);
            size_t           start = 0;
            size_t           end   = line_view.find(',');

            while (end != std::string_view::npos)
            {
                separated_cells_from_line.push_back(line_view.substr(start, end - start));
                start = end + 1;
                end   = line_view.find(',', start);
            }
            separated_cells_from_line.push_back(line_view.substr(start));


            const size_t size_of_separated_cells_from_line = separated_cells_from_line.size();
            if (date_index >= size_of_separated_cells_from_line || time_index >= size_of_separated_cells_from_line ||
                collision_year_index >= size_of_separated_cells_from_line ||
                number_of_vehicles_index >= size_of_separated_cells_from_line ||
                number_of_casualties_index >= size_of_separated_cells_from_line ||
                weather_conditions_index >= size_of_separated_cells_from_line ||
                junction_detail_index >= size_of_separated_cells_from_line ||
                collision_severity_index >= size_of_separated_cells_from_line ||
                speed_limit_index >= size_of_separated_cells_from_line ||
                pedestrian_crossing_index >= size_of_separated_cells_from_line ||
                day_of_week_index >= size_of_separated_cells_from_line)
            {
                std::string error_msg = "Index out of range, max expected index is: {" +
                    std::to_string(size_of_separated_cells_from_line) +
                    "} got values \n[\ndate_index : " + std::to_string(date_index) + "\n" +
                    "time_index : " + std::to_string(time_index) + "\n" +
                    "collision_year_index : " + std::to_string(collision_year_index) + "\n" +
                    "number_of_vehicles_index : " + std::to_string(number_of_vehicles_index) + "\n" +
                    "number_of_casualties_index : " + std::to_string(number_of_casualties_index) + "\n" +
                    "weather_conditions_index : " + std::to_string(weather_conditions_index) + "\n" +
                    "junction_detail_index :" + std::to_string(junction_detail_index) + "\n" +
                    "collision_severity_index : " + std::to_string(collision_severity_index) + "\n" +
                    "speed_limit_index :" + std::to_string(speed_limit_index) + "\n" +
                    "pedestrian_crossing_index :" + std::to_string(pedestrian_crossing_index) + "\n" +
                    "day_of_week_index :" + std::to_string(day_of_week_index) + "\n]";
                throw std::runtime_error(error_msg);
            }

            Accident to_raw_data{};

            // =========
            // Date
            // =========
            this->handle_date(to_raw_data, separated_cells_from_line[date_index]);

            // =========
            // Time
            // =========
            this->handle_time(to_raw_data, separated_cells_from_line[time_index]);

            // =========
            // Year
            // =========
            to_raw_data.collision_year = std::stoi(std::string(separated_cells_from_line[collision_year_index]));

            // =========
            // Number of Vehicles
            // =========
            to_raw_data.number_of_vehicles =
                std::stoi(std::string(separated_cells_from_line[number_of_vehicles_index]));

            // =========
            // Number of Casualties
            // =========
            to_raw_data.number_of_casualties =
                std::stoi(std::string(separated_cells_from_line[number_of_casualties_index]));

            // =========
            // Weather Conditions
            // =========
            this->handle_weather_conditions(to_raw_data, separated_cells_from_line[weather_conditions_index]);

            // =========
            // Junction Detail
            // =========
            this->handle_junction_details(to_raw_data, separated_cells_from_line[junction_detail_index]);

            // =========
            // Collision Type
            // =========
            this->handle_collision_severity(to_raw_data, separated_cells_from_line[collision_severity_index]);

            // =========
            // Speed Limit
            // =========
            to_raw_data.speed_limit = std::stoi(std::string(separated_cells_from_line[speed_limit_index]));

            // =========
            // Pedestrian Crossing
            // =========
            this->handle_pedestrian_crossing(to_raw_data, separated_cells_from_line[pedestrian_crossing_index]);

            // =========
            // Pedestrian Crossing
            // =========
            this->handle_day_of_week(to_raw_data, separated_cells_from_line[day_of_week_index]);

            // Implicit mem
            this->m_data_frame.handle_incoming_data(to_raw_data);
        }
    }

    // =====================
    // Handle Date Function
    // =====================
    template <typename T>
    void CSV_Loader<T>::handle_date(Accident& data, std::string_view date_from_separated_cells)
    {
        const size_t first_slash  = date_from_separated_cells.find('/');
        const size_t second_slash = date_from_separated_cells.find('/', first_slash + 1);

        if (first_slash != std::string_view::npos && second_slash != std::string_view::npos)
        {
            try
            {
                const int day   = std::stoi(std::string(date_from_separated_cells.substr(0, first_slash)));
                const int month = std::stoi(
                    std::string(date_from_separated_cells.substr(first_slash + 1, second_slash - first_slash - 1)));
                const int year = std::stoi(std::string(date_from_separated_cells.substr(second_slash + 1)));
                data.date      = std::chrono::year{year} / month / day;
            }
            catch (...)
            {
                throw std::runtime_error("Wrong format of date, expected: {DD/MM/YYYY}");
            }
        }
        else
        {
            throw std::runtime_error("Wrong format of date, expected: {DD/MM/YYYY}");
        }
    }

    // =====================
    // Handle Time Function
    // =====================
    template <typename T>
    void CSV_Loader<T>::handle_time(Accident& data, std::string_view time_from_separated_cells)
    {
        const size_t colon = time_from_separated_cells.find(':');

        if (colon != std::string_view::npos)
        {
            try
            {
                const int hour   = std::stoi(std::string(time_from_separated_cells.substr(0, colon)));
                const int minute = std::stoi(std::string(time_from_separated_cells.substr(colon + 1)));
                data.time        = std::chrono::hours(hour) + std::chrono::minutes(minute);
            }
            catch (...)
            {
                throw std::runtime_error("Wrong format of time, expected: {HH:MM}");
            }
        }
        else
        {
            throw std::runtime_error("Wrong format of time, expected: {HH:MM}");
        }
    }

    // =======================
    // Handle Weather Function
    // =======================
    template <typename T>
    void CSV_Loader<T>::handle_weather_conditions(Accident& data, const std::string_view weather_from_separated_cells)
    {
        switch (std::stoi(std::string(weather_from_separated_cells)))
        {
            case -1:
                data.weather_conditions = Weather::unmarked_data;
                break;
            case 1:
                data.weather_conditions = Weather::fine_no_high_winds;
                break;
            case 2:
                data.weather_conditions = Weather::raining_no_high_winds;
                break;
            case 3:
                data.weather_conditions = Weather::snowing_no_high_winds;
                break;
            case 4:
                data.weather_conditions = Weather::fine_high_winds;
                break;
            case 5:
                data.weather_conditions = Weather::raining_high_winds;
                break;
            case 6:
                data.weather_conditions = Weather::snowing_high_winds;
                break;
            case 7:
                data.weather_conditions = Weather::fog_or_mist;
                break;
            case 8:
                data.weather_conditions = Weather::other;
                break;
            case 9:
                data.weather_conditions = Weather::unknown;
                break;
            default:
                throw std::runtime_error(
                    "Wrong weather_conditions data, expected from { 1 to 9 } got {" +
                    std::string(weather_from_separated_cells) + "}");
        }
    }

    // ========================
    // Handle Junction Function
    // ========================
    template <typename T>
    void CSV_Loader<T>::handle_junction_details(Accident& data, const std::string_view junction_from_separated_cells)
    {
        switch (std::stoi(std::string(junction_from_separated_cells)))
        {
            case 0:
                data.junction_detail = Junction::not_at_junction_or_within_20_meters;
                break;
            case 13:
                data.junction_detail = Junction::t_or_staggered_junction;
                break;
            case 16:
                data.junction_detail = Junction::crossroads;
                break;
            case 17:
                data.junction_detail = Junction::junction_with_more_than_foru_arms_not_roundabout;
                break;
            case 18:
                data.junction_detail = Junction::private_drive_or_entrence;
                break;
            case 19:
                data.junction_detail = Junction::data_missing_or_out_of_range;
                break;
            case 99:
                data.junction_detail = Junction::unknown;
                break;
            case -1:
                data.junction_detail = Junction::data_missing_or_out_of_range;
                break;
            default:
                throw std::runtime_error(
                    "Wrong junction_detail data, expected  { 0 : 13 : 16 : 17 : 18 : 99 : -1 } got {" +
                    std::string(junction_from_separated_cells) + "}");
        }
    }

    // =========================
    // Handle Collision Function
    // =========================
    template <typename T>
    void CSV_Loader<T>::handle_collision_severity(Accident& data, const std::string_view collision_from_separated_cells)
    {
        switch (std::stoi(std::string(collision_from_separated_cells)))
        {
            case 1:
                data.collision_severity = Collision_Severity::killed;
                break;
            case 2:
                data.collision_severity = Collision_Severity::serious;
                break;
            case 3:
                data.collision_severity = Collision_Severity::slight;
                break;
            case 4:
                data.collision_severity = Collision_Severity::unknown;
                break;

            default:
                throw std::runtime_error(
                    "Wrong collision_severity data, expected  { 1 to 4 } got {" +
                    std::string(collision_from_separated_cells) + "}");
        }
    }

    template <typename T>
    void
    CSV_Loader<T>::handle_pedestrian_crossing(Accident& data, const std::string_view pedestrian_from_separated_cells)
    {
        switch (std::stoi(std::string(pedestrian_from_separated_cells)))
        {
            case -1:
                data.pedestrian_crossing = Pedestrian_Crossing::data_missing_or_out_of_range;
                break;
            case 0:
                data.pedestrian_crossing = Pedestrian_Crossing::no_physical_crossing_facility_within_50m;
                break;
            case 11:
                data.pedestrian_crossing = Pedestrian_Crossing::human_crossing_control_by_school_crossing_patrol;
                break;
            case 12:
                data.pedestrian_crossing = Pedestrian_Crossing::human_crossing_control_by_other_authorised_person;
                break;
            case 13:
                data.pedestrian_crossing = Pedestrian_Crossing::zebra_crossing;
                break;
            case 14:
                data.pedestrian_crossing = Pedestrian_Crossing::pedestrian_light_crossing;
                break;
            case 15:
                data.pedestrian_crossing = Pedestrian_Crossing::pedestrian_phase_at_traffic_signal;
                break;
            case 16:
                data.pedestrian_crossing = Pedestrian_Crossing::footbridge_or_subway;
                break;
            case 17:
                data.pedestrian_crossing = Pedestrian_Crossing::central_refuge;
                break;
            case 99:
                data.pedestrian_crossing = Pedestrian_Crossing::unknown;
                break;
            default:
                throw std::runtime_error(
                    "Wrong pedestrian_crossing data, expected  { 11 to 17 and 0 & 99 } got {" +
                    std::string(pedestrian_from_separated_cells) + "}");
        }
    }

    template <typename T>
    void CSV_Loader<T>::handle_day_of_week(Accident& data, const std::string_view day_of_week_from_separated_cells)
    {
        switch (std::stoi(std::string(day_of_week_from_separated_cells)))
        {
            case -1:
                data.day_of_week = Day_Of_Week::data_missing_or_out_of_range;
                break;
            case 1:
                data.day_of_week = Day_Of_Week::sunday;
                break;
            case 2:
                data.day_of_week = Day_Of_Week::monday;
                break;
            case 3:
                data.day_of_week = Day_Of_Week::tuesday;
                break;
            case 4:
                data.day_of_week = Day_Of_Week::wednesday;
                break;
            case 5:
                data.day_of_week = Day_Of_Week::thursday;
                break;
            case 6:
                data.day_of_week = Day_Of_Week::friday;
                break;
            case 7:
                data.day_of_week = Day_Of_Week::saturday;
                break;

            default:
                throw std::runtime_error(
                    "Wrong day_of_week data, expected  { 1 to 7 } got {" +
                    std::string(day_of_week_from_separated_cells) + "}");
        }
    }


} // namespace aus


#endif // AUS_CSV_LOADER_HPP
