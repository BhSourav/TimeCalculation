//
// Created by Sourav Bhattacharjee on 27.07.25.
//

export module timebase;

export namespace Time {
    /**
     * @brief Defines standard units of time for calculations and conversions.
     *
     * This enumeration provides a type-safe way to specify time units,
     * ranging from hours down to microseconds. It is used throughout the
     * time calculation library to ensure clarity and prevent errors related
     * to ambiguous time representations.
     */
    enum class TimeUnits {
        HOURS,         ///< Represents a unit of time in hours.
        MINUTES,       ///< Represents a unit of time in minutes.
        SECONDS,       ///< Represents a unit of time in seconds.
        MILLISECONDS,  ///< Represents a unit of time in milliseconds.
        MICROSECONDS   ///< Represents a unit of time in microseconds.
    };

}
