//
// Created by Sourav Bhattacharjee on 28.07.25.
//
#include <gmock/gmock.h>
#include <chrono>

import timebase;

namespace Time::Testing {

    template<TimeUnit unit = TimeUnit::SECONDS>
    class MockTimeBase {
    public:
        virtual ~MockTimeBase() = default;

        using Duration = std::chrono::duration<double, TimeType<unit>>;

        MOCK_METHOD(std::string_view, TimeUnit, (), (const));
        MOCK_METHOD(Duration, elapsed, (), (const));
    };

} // namespace Time::Testing