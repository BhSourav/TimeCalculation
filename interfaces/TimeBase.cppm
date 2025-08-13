//
// Created by Sourav Bhattacharjee on 27.07.25.
//
module;
#include <chrono>
#include <type_traits>

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
    enum class TimeUnit {
        HOURS, ///< Represents a unit of time in hours.
        MINUTES, ///< Represents a unit of time in minutes.
        SECONDS, ///< Represents a unit of time in seconds.
        MILLISECONDS, ///< Represents a unit of time in milliseconds.
        MICROSECONDS ///< Represents a unit of time in microseconds.
    };

    /**
    * @brief A type alias that maps a TimeUnits enumerator to its corresponding std::chrono duration type.
 * @details This alias uses template metaprogramming with std::conditional_t to select the
 *          appropriate std::chrono duration at compile time. For example, `TimeType<TimeUnits::SECONDS>`
 *          resolves to `std::chrono::seconds`.
 * @tparam e The TimeUnits enumerator that determines the resulting std::chrono type.
     */
    template<TimeUnit e>
    using TimeType = std::conditional_t<e == TimeUnit::HOURS, std::ratio<3600>,
        std::conditional_t<e == TimeUnit::MINUTES, std::ratio<60>,
            std::conditional_t<e == TimeUnit::SECONDS, std::ratio<1>,
                std::conditional_t<e == TimeUnit::MILLISECONDS, std::milli,
                    std::micro> > > >;

    /**
 * @class TimeBase
 * @brief A versatile timer for measuring elapsed time with configurable units.
 *
 * @details This class provides a simple yet powerful way to measure time intervals.
 *          Upon construction, it captures the current time using `std::chrono::steady_clock`,
 *          which is a monotonic clock suitable for measuring time durations. The elapsed
 *          time can then be retrieved in various units specified by the `TimeUnits`
 *          template parameter. The result is returned as a floating-point `std::chrono::duration`
 *          for high precision.
 *
 * @tparam unit The unit of time for measuring and reporting the elapsed duration.
 *              Defaults to `TimeUnits::SECONDS`.
 *
 * @code
 * #include <iostream>
 * #include <thread>
 *
 * // Assuming the module is imported
 *  import timebase;
 *
 * int main() {
 *     // Create a timer that measures in milliseconds
 *     Time::TimeBase<Time::TimeUnits::MILLISECONDS> timer;
 *
 *     std::cout << "Timer started. Unit: " << timer.TimeUnit() << std::endl;
 *
 *     // Simulate some work for about 1234 milliseconds
 *     std::this_thread::sleep_for(std::chrono::milliseconds(1234));
 *
 *     // Get the elapsed time.
 *     // Note: elapsed() could be called multiple times to check time at different points.
 *     auto elapsed_ms = timer.elapsed();
 *
 *     std::cout << "Elapsed time: " << elapsed_ms.count() << " ms" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
    template<TimeUnit unit = TimeUnit::SECONDS>
    class TimeBase {
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_startTime_;
        std::chrono::time_point<std::chrono::steady_clock> m_stopTime_;
        bool is_running_;

        [[nodiscard]] constexpr const char *Units_to_string(const TimeUnit e) const {
            switch (e) {
                case TimeUnit::HOURS: return "HOURS";
                case TimeUnit::MINUTES: return "MINUTES";
                case TimeUnit::SECONDS: return "SECONDS";
                case TimeUnit::MILLISECONDS: return "MILLISECONDS";
                case TimeUnit::MICROSECONDS: return "MICROSECONDS";
                default: return "unknown";
            }
        }

    public:
        /**
    * @brief Constructs a TimeBase object and starts the timer.
    * @details The constructor captures the current time point from `std::chrono::steady_clock`,
    *          which serves as the starting point for all subsequent elapsed time measurements.
    */
        TimeBase() : m_startTime_(std::chrono::steady_clock::now())
                    , is_running_(true) {}

        /** @brief Default destructor. */
        virtual ~TimeBase() = default;

        /** @brief Default copy constructor. */
        TimeBase(const TimeBase &) = default;

        /** @brief Default copy assignment operator. */
        TimeBase &operator=(const TimeBase &) = default;

        /** @brief Default move constructor. */
        TimeBase(TimeBase &&) = default;

        /** @brief Default move assignment operator. */
        TimeBase &operator=(TimeBase &&) = default;

        using Duration = std::chrono::duration<double, TimeType<unit>>;

        /**
         * @brief Gets the absolute time point when the timer was started.
         * @return The std::chrono::time_point of the start time.
         */
        [[nodiscard]] virtual std::chrono::time_point<std::chrono::steady_clock> startTime() const {
            return m_startTime_;
        }

        /**
         * @brief Gets the absolute time point when the timer was stopped.
         * @return The std::chrono::time_point of the stop time.
         * @warning If the timer is still running, the returned value is uninitialized.
         *          Check isRunning() before calling.
         */
        [[nodiscard]] virtual std::optional<std::chrono::time_point<std::chrono::steady_clock>> stopTime() const {
            if (is_running_)
                return std::nullopt;
            return m_stopTime_;
        }

        /**
         * @brief Checks if the timer is currently running.
         * @return True if the timer is running, false if stop() has been called.
         */
        [[nodiscard]] virtual bool isRunning() const {
            return is_running_;
        }

        /**
     * @brief Returns the time unit of the timer as a string.
     * @return A `std::string_view` representing the configured time unit (e.g., "SECONDS").
     */
        [[nodiscard]] virtual std::string_view TimeUnit() const { return Units_to_string(unit); }
        /**
 * @brief Calculates the elapsed time since the timer was started.
 * @details This method calculates the duration between the current time and the time
 *          the `TimeBase` object was constructed. It can be called multiple times.
 * @return A `std::chrono::duration` with a `double` representation holding the elapsed
 *         time in the unit specified by the template parameter `unit`.
 */
        [[nodiscard]] virtual Duration elapsed() const {
            if (is_running_)
                return Duration(std::chrono::steady_clock::now() - m_startTime_);
            return Duration(m_stopTime_ - m_startTime_);
        }

        virtual void stop() {
            is_running_ = false;
            m_stopTime_ = std::chrono::steady_clock::now();
        }
    };
}
