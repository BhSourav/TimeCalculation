//
// Created by Sourav Bhattacharjee on 28.07.25.
//
#include <gmock/gmock.h>
#include <chrono>

import timebase;

namespace Time::Testing {

    template<TimeUnit unit = TimeUnit::SECONDS>
    class MockTimeBase : public Time::TimeBase<unit>{
    public:
        //~MockTimeBase() override = default;

        using Duration = typename TimeBase<unit>::Duration;

        MOCK_METHOD(std::string_view, TimeUnit, (), (const, override));
        MOCK_METHOD(Duration, elapsed, (), (const, override));
        MOCK_METHOD(void, stop, (), (override));
        MOCK_METHOD(std::chrono::time_point<std::chrono::steady_clock>, startTime, (), (const, override));
        MOCK_METHOD(bool, isRunning, (), (const, override));
        MOCK_METHOD(std::optional<std::chrono::time_point<std::chrono::steady_clock>>, stopTime, (), (const, override));
    };

} // namespace Time::Testing