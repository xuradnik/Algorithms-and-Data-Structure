#ifndef AUS_ACCIDENT_HPP
#define AUS_ACCIDENT_HPP

#include <chrono>

/**
 * @file accident.hpp
 * @brief Data types for a road traffic accident record.
 *
 * Holds the enums for categorical fields and the Accident struct
 * with every field loaded from the CSV dataset.
 */

namespace aus
{
    /**
     * @brief Weather at the time of the accident.
     */
    enum class Weather : uint8_t
    {
        unmarked_data         = 0, ///< CSV value: -1 (no data recorded)
        fine_no_high_winds    = 1, ///< CSV value: 1
        raining_no_high_winds = 2, ///< CSV value: 2
        snowing_no_high_winds = 3, ///< CSV value: 3
        fine_high_winds       = 4, ///< CSV value: 4
        raining_high_winds    = 5, ///< CSV value: 5
        snowing_high_winds    = 6, ///< CSV value: 6
        fog_or_mist           = 7, ///< CSV value: 7
        other                 = 8, ///< CSV value: 8
        unknown               = 9  ///< CSV value: 9
    };

    /**
     * @brief Junction type at the accident location.
     */
    enum class Junction : uint8_t
    {
        data_missing_or_out_of_range                     = 0, ///< CSV value: -1
        not_at_junction_or_within_20_meters              = 1, ///< CSV value: 0
        t_or_staggered_junction                          = 2, ///< CSV value: 13
        crossroads                                       = 3, ///< CSV value: 16
        junction_with_more_than_foru_arms_not_roundabout = 4, ///< CSV value: 17
        private_drive_or_entrence                        = 5, ///< CSV value: 18
        unknown                                          = 6, ///< CSV value: 99
    };

    /**
     * @brief Collision severity.
     */
    enum class Collision_Severity : uint8_t
    {
        killed  = 0, ///< CSV value: 1
        serious = 1, ///< CSV value: 2
        slight  = 2, ///< CSV value: 3
        unknown = 3  ///< CSV value: 4
    };

    /**
     * @brief Pedestrian crossing near the accident.
     */
    enum Pedestrian_Crossing : uint8_t
    {
        data_missing_or_out_of_range                      = 0, ///< CSV value: -1
        no_physical_crossing_facility_within_50m          = 1, ///< CSV value: 0
        human_crossing_control_by_school_crossing_patrol  = 2, ///< CSV value: 11
        human_crossing_control_by_other_authorised_person = 3, ///< CSV value: 12
        zebra_crossing                                    = 4, ///< CSV value: 13
        pedestrian_light_crossing                         = 5, ///< CSV value: 14
        pedestrian_phase_at_traffic_signal                = 6, ///< CSV value: 15
        footbridge_or_subway                              = 7, ///< CSV value: 16
        central_refuge                                    = 8, ///< CSV value: 17
        unknown                                           = 9  ///< CSV value: 99
    };

    /**
     * @brief Day of the week of the accident.
     */
    enum class Day_Of_Week : uint8_t
    {
        data_missing_or_out_of_range = 0, ///< CSV value: -1
        sunday                       = 1, ///< CSV value: 1
        monday                       = 2, ///< CSV value: 2
        tuesday                      = 3, ///< CSV value: 3
        wednesday                    = 4, ///< CSV value: 4
        thursday                     = 5, ///< CSV value: 5
        friday                       = 6, ///< CSV value: 6
        saturday                     = 7  ///< CSV value: 7
    };


    /**
     * @brief Record for a single road-traffic accident.
     */
    struct Accident
    {
        std::chrono::minutes        time;                 ///< Minutes since midnight. 8 bytes.
        std::chrono::year_month_day date;                 ///< Accident date. 4 bytes.
        uint16_t                    collision_year;       ///< Year from the collision record. 2 bytes.
        uint16_t                    speed_limit;          ///< Speed limit at the location. 2 bytes.
        uint8_t                     number_of_vehicles;   ///< Vehicles involved. 1 byte.
        uint8_t                     number_of_casualties; ///< Casualties. 1 byte.
        Weather                     weather_conditions;   ///< Weather. 1 byte.
        Junction                    junction_detail;      ///< Junction type. 1 byte.
        Collision_Severity          collision_severity;   ///< Severity. 1 byte.
        Pedestrian_Crossing         pedestrian_crossing;  ///< Nearest pedestrian crossing. 1 byte.
        Day_Of_Week                 day_of_week;          ///< Day of week. 1 byte.
    };
} // namespace aus

#endif // AUS_ACCIDENT_HPP
