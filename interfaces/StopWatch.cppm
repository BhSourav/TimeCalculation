//
// Created by Sourav Bhattacharjee on 29.07.25.
//
module;
#include <chrono>
export module stopwatch;
import timebase;
export namespace Time {
    /**
     * @class StopWatch
     * @brief A stopwatch utility to measure elapsed time with lap functionality.
     * @tparam unit The time unit for measurement (e.g., SECONDS, MILLISECONDS).
     *
     * This class provides basic stopwatch features: start, lap, and stop (implicitly on lap).
     * It stores the time for each lap individually.
     */
    template<TimeUnit unit = TimeUnit::SECONDS>
    class StopWatch {
        public:
        /**
         * @brief Default constructor.
         */
        StopWatch() = default;
        /**
         * @brief Default destructor.
         */
        virtual ~StopWatch() noexcept = default;

        // Making the class move-only since it contains unique_ptr.
        StopWatch(const StopWatch &) = delete;
        StopWatch &operator=(const StopWatch &) = delete;
        StopWatch(StopWatch &&) noexcept = default;
        StopWatch &operator=(StopWatch &&) noexcept = default;

        /**
         * @brief Starts the stopwatch.
         *
         * Initializes the stopwatch and starts timing the first lap.
         * This must be called before any other timing functions.
         * @return An optional string containing an error message if the watch has already been started,
         *         otherwise std::nullopt.
         */
        virtual std::optional<std::string> start() {
            if (!m_stopWatch_.empty())
                return "Watch is already running and must be reset first.";
            m_stopWatch_.reserve(10);
            m_stopWatch_.emplace_back(std::make_unique<TimeBase<unit>>());
            return std::nullopt;
        }

        /**
         * @brief Records a lap and starts the next one.
         *
         * Stops the timer for the current lap and immediately starts a new one.
         * @return An optional string containing an error message if the watch hasn't been started
         *         or if the lap limit is exceeded, otherwise std::nullopt.
         */
        virtual std::optional<std::string> lap() {
            if (m_stopWatch_.empty())
                return "The stopwatch should be initialised with the start() function.";
            if (m_stopWatch_.size() >= MAX_LAP)
                return "Lap count exceeded";
            m_stopWatch_.back()->stop();
            m_stopWatch_.emplace_back(std::make_unique<TimeBase<unit>>());
            return std::nullopt;
        }

        /**
         * @brief Records a lap and returns its duration.
         *
         * Stops the timer for the current lap, returns its duration, and immediately starts a new lap.
         * @return A std::variant containing the lap duration on success, or an error string on failure.
         */
        virtual std::variant<std::chrono::duration<double, TimeType<unit>>, std::string> lapWithReturnTime() {
            if (m_stopWatch_.empty())
                return "The stopwatch should be initialised with the start() function.";
            if (m_stopWatch_.size() >= MAX_LAP)
                return "Lap count exceeded";
            auto& currentLap = m_stopWatch_.back();
            currentLap->stop();
            m_stopWatch_.emplace_back(std::make_unique<TimeBase<unit>>());
            return currentLap.elapsed();
        }

        /**
        * @brief Gets the current lap number.
        *
        * The first lap is lap 1. If the stopwatch has not been started, this returns 0.
        *
        * @return The current lap number being timed.
        */
        virtual size_t lapNumber() const noexcept {
            return m_stopWatch_.size();
        }

        /**
         * @brief Resets the stopwatch to its initial state.
         *
         * Clears all recorded laps and stops the timer.
         */
        void reset() noexcept {
            m_stopWatch_.clear();
        }

        /**
         * @brief Gets the number of fully completed laps.
         * @return The count of completed laps.
         */
        size_t completedLapCount() const noexcept {
            if (m_stopWatch_.empty())
                return 0;
            return m_stopWatch_.size() - 1;
        }

        /**
         * @brief Gets the duration of a specific completed lap.
         * @param lapNum The 1-based lap number to query.
         * @return A std::variant containing the lap's duration on success, or an error string on failure.
         */
        virtual std::variant<std::chrono::duration<double, TimeType<unit>>, std::string> lapTime(const uint8_t lapNum) const {
            const auto completedLaps = completedLapCount();
            if (lapNum == 0 || lapNum > completedLaps)
                return "Invalid lap number. Must be between 1 and " + std::to_string(completedLaps) + ".";
            return m_stopWatch_[lapNum - 1]->elapsed();
        }

        /**
         * @brief Calculates the total elapsed time from the start to the end of the last completed lap.
         * @return A std::variant containing the total duration on success, or an error string if no laps are complete.
         */
        virtual std::variant<std::chrono::duration<double, TimeType<unit>>, std::string> totalTime() const {
            const auto completedLaps = completedLapCount();
            if (completedLaps == 0) {
                return "No completed laps to measure.";
            }
            return timeBetweenLaps(1, static_cast<uint8_t>(completedLaps));
        }

        /**
         * @brief Calculates the total elapsed time between any two completed laps.
         *
         * This function provides an optimized calculation for the duration spanning
         * from the beginning of a start lap to the end of an end lap.
         *
         * @param startLap The lap number (1-based) to start measuring from.
         * @param endLap The lap number (1-based) to stop measuring at. Must be a completed lap.
         * @return A std::variant containing the total duration on success, or an error string on failure.
         */
        virtual std::variant<std::chrono::duration<double, TimeType<unit>>, std::string>
        timeBetweenLaps(const uint8_t startLap, uint8_t endLap) const {
            // --- Validation ---
            if (startLap == 0 || endLap == 0) {
                return "Lap numbers must be 1-based and non-zero.";
            }
            if (startLap > endLap) {
                return "Start lap must be less than or equal to end lap.";
            }

            if (const auto completedLaps = completedLapCount(); endLap > completedLaps) {
                return "End lap " + std::to_string(endLap) + " has not been completed. "
                       "Highest completed lap is " + std::to_string(completedLaps) + ".";
            }

            const auto& firstLap = m_stopWatch_[startLap - 1];
            const auto& lastLap = m_stopWatch_[endLap - 1];

            // Get the absolute start and stop times.
            const auto absoluteStartTime = firstLap->startTime();
            const auto absoluteStopTime = lastLap->stopTime();

            return std::chrono::duration<double, TimeType<unit>>(absoluteStopTime - absoluteStartTime);
        }

    private:
        /// @brief A list of time measurements, where each element represents a single lap.
        std::vector<std::unique_ptr<TimeBase<unit>>> m_stopWatch_;
        /// @brief The maximum number of laps the stopwatch can record.
        static constexpr uint8_t MAX_LAP = 200;
    };
}
