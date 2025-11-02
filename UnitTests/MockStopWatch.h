//
// Created by Sourav Bhattacharjee on 29.08.25.
//

#ifndef MOCKSTOPWATCH_H
#define MOCKSTOPWATCH_H

#include <build/debug/_deps/googletest-src/googlemock/include/gmock/gmock.h>
import stopwatch;
import timebase;
namespace Time::Testing {
    template<Time::TimeUnit unit = TimeUnit::SECONDS>
    class MockStopWatch : public StopWatch<unit> {
        MOCK_METHOD(std::optional<std::string>, start, (), (override));
    };
}

#endif //MOCKSTOPWATCH_H
