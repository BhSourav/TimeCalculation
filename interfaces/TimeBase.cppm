//
// Created by Sourav Bhattacharjee on 27.07.25.
//

export module timebase;
#include <chrono>
#include <type_traits>

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
        HOURS, ///< Represents a unit of time in hours.
        MINUTES, ///< Represents a unit of time in minutes.
        SECONDS, ///< Represents a unit of time in seconds.
        MILLISECONDS, ///< Represents a unit of time in milliseconds.
        MICROSECONDS ///< Represents a unit of time in microseconds.
    };

    constexpr char *Units_to_string(const TimeUnits e) {
        switch (e) {
            case TimeUnits::HOURS: return "HOURS";
            case TimeUnits::MINUTES: return "MINUTES";
            case TimeUnits::SECONDS: return "SECONDS";
            case TimeUnits::MILLISECONDS: return "MILLISECONDS";
            case TimeUnits::MICROSECONDS: return "MICROSECONDS";
            default: return "unknown";
        }
    }

    /**
    * @brief A type alias that maps a TimeUnits enumerator to its corresponding std::chrono duration type.
 * @details This alias uses template metaprogramming with std::conditional_t to select the
 *          appropriate std::chrono duration at compile time. For example, `TimeType<TimeUnits::SECONDS>`
 *          resolves to `std::chrono::seconds`.
 * @tparam e The TimeUnits enumerator that determines the resulting std::chrono type.
     */
    template<TimeUnits e>
    using TimeType = std::conditional_t<e == TimeUnits::HOURS, std::chrono::hours,
        std::conditional_t<e == TimeUnits::MINUTES, std::chrono::minutes,
        std::conditional_t<e == TimeUnits::SECONDS, std::chrono::seconds,
        std::conditional_t<e == TimeUnits::MILLISECONDS, std::chrono::milliseconds, std::chrono::microseconds>>>>;

    template<TimeUnits unit = TimeUnits::SECONDS>
    class Time {
        private:
        TimeType<unit> timeUnit_;
        public:
        Time() : timeUnit_(unit) {};
        ~Time() = default;
        Time(const Time &) = default;
        Time &operator=(const Time &) = default;
        Time(Time &&) = default;
        Time &operator=(Time &&) = default;

        std::string_view TimeUnit() { return Units_to_string(unit); };
    };
}
