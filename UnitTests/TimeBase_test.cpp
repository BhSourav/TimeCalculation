//
// Created by Sourav Bhattacharjee on 28.07.25.
//
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

import timebase;

TEST(TimeBaseTest, TimeUnitStringValue) {
    Time::TimeBase<Time::TimeUnit::MINUTES> time_min;
    EXPECT_EQ(time_min.TimeUnit(), "MINUTES");
    Time::TimeBase time_sec;
    EXPECT_EQ(time_sec.TimeUnit(), "SECONDS");
    Time::TimeBase<Time::TimeUnit::MILLISECONDS> time_ms;
    EXPECT_EQ(time_ms.TimeUnit(), "MILLISECONDS");
}

TEST(TimeBaseTest, ElapsedTimeIsIncreasing) {
    Time::TimeBase<Time::TimeUnit::MICROSECONDS> timer;
    auto time1 = timer.elapsed().count();
    EXPECT_GE(time1, 0);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    auto time2 = timer.elapsed().count();
    EXPECT_GT(time2, time1);
}

TEST(TimeBaseTest, DefaultUnitIsSecond) {
    Time::TimeBase timer;
    EXPECT_EQ(timer.TimeUnit(), "SECONDS");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto elapsed = timer.elapsed().count();
    EXPECT_GE(elapsed, 1);
    EXPECT_LT(elapsed, 1.1);
}
