//
// Created by Sourav Bhattacharjee on 29.07.25.
//
module;
#include <chrono>
#include <list>
export module stopwatch;
import timebase;
export namespace Time {
    template<Time::TimeUnit unit = TimeUnit::SECONDS>
    class StopWatch {
        public:
        StopWatch() = default;
        virtual ~StopWatch() = default;
        StopWatch(const StopWatch &) = default;
        StopWatch &operator=(const StopWatch &) = default;
        StopWatch(StopWatch &&) = default;
        StopWatch &operator=(StopWatch &&) = default;
        std::optional<std::string> start() {
            if (!m_stopWatch_.empty())
                return "Watch is not empty";
            m_stopWatch_.emplace_front(std::make_unique<TimeBase<unit>>());
            m_currentLap++;
        }
        std::optional<std::string> lap() {
            if (m_stopWatch_.empty())
                return "The stopwatch should be initialised with the start() function.";
            if (m_currentLap >= MAX_LAP)
                return "Lap count exceeded";
            m_stopWatch_.end()->stop();
            m_stopWatch_.emplace_back(std::make_unique<TimeBase<unit>>());
            m_currentLap++;
        }
        std::variant<std::chrono::duration<double, TimeType<unit>>, std::string> lapWithReturnTime() {
            if (m_stopWatch_.empty())
                return "The stopwatch should be initialised with the start() function.";
            if (m_currentLap >= MAX_LAP)
                return "Lap count exceeded";
            m_stopWatch_.end()->stop()
            std::chrono::duration<double, TimeType<unit>> t = m_stopWatch_.end()->elapsed();
            m_stopWatch_.emplace_back(std::make_unique<TimeBase<unit>>());
            m_currentLap++;
            return t;
        }
    private:
        std::list<std::unique_ptr<TimeBase<unit>>> m_stopWatch_;
        static constexpr uint8_t MAX_LAP = 200;
        uint8_t m_currentLap = 0;
    };
}
