//
// Created by Sourav.Bhattacharjee on 21.08.2025.
//

#include <gtest/gtest.h>
#include "MockAlarm.h"
import timebase;
import alarm;

TEST(AlarmTest, AlarmFinishesOnTime) {
    auto timer = std::make_unique<Time::Alarm<Time::TimeUnit::MILLISECONDS>>(std::chrono::milliseconds(100));
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(timer->shouldAlarmRing());
}

TEST(AlarmTest, AlarmCanBeStoppedEarly) {
    auto timer = std::make_unique<Time::Alarm<>>(std::chrono::seconds(5));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    timer->stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_FALSE(timer->shouldAlarmRing());
}

TEST(AlarmTest, MoveConstructorWorks) {
    auto timer1 = std::make_unique<Time::Alarm<>>(std::chrono::seconds(3));
    ASSERT_TRUE(timer1->isJoinable());
    Time::Alarm<> timer2(std::move(*timer1));
    ASSERT_FALSE(timer1->isJoinable());
    ASSERT_TRUE(timer2.isJoinable());
    std::this_thread::sleep_for(std::chrono::seconds(4));
    ASSERT_TRUE(timer2.shouldAlarmRing());
}

TEST(AlarmTest, MoveAssignmentOperatorWorks) {
    auto timer1 = std::make_unique<Time::Alarm<>>(std::chrono::seconds(3));
    Time::Alarm<> timer2(std::chrono::seconds(1));
    timer2 = std::move(*timer1);
    ASSERT_FALSE(timer1->isJoinable());
    ASSERT_TRUE(timer2.isJoinable());
    std::this_thread::sleep_for(std::chrono::seconds(4));
    ASSERT_TRUE(timer2.shouldAlarmRing());
}

// --- Gmock Unit Test for the MockTimer ---
TEST(MockAlarmTest, TaskCompletesWhenMockAlarmIsFinished)
{
    Time::Testing::MockAlarm<> mock_timer(std::chrono::seconds(3));
    Time::Testing::Task<> task(mock_timer);

    // Set an expectation: we expect the `isFinished` method to be called twice.
    // The first time it will return false, the second time it will return true.
    using ::testing::Return;
    EXPECT_CALL(mock_timer, shouldAlarmRing())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    task.execute();
}