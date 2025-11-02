//
// Created by Sourav Bhattacharjee on 13.08.25.
//
#include <gmock/gmock.h>
#include <chrono>

import stopwatch;
import timebase;
namespace Time::Testing {
    template<TimeUnit unit = TimeUnit::SECONDS>
    class MockStopWatch : public StopWatch<unit> {
        //MOCK_METHOD(std::optional<std::string>, start, (), (override));
        MOCK_METHOD(std::optional<std::string>, lap, (), (override));
    };
}