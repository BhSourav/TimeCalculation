//
// Created by Sourav.Bhattacharjee on 21.08.2025.
//
#ifndef MOCKALARM_H
#define MOCKALARM_H

#include <gmock/gmock.h>
#include <mutex>
#include <thread>
import timebase;
import alarm;

namespace Time::Testing {
template <Time::TimeUnit Unit = Time::TimeUnit::SECONDS>
class MockAlarm : public Time::Alarm<Unit>
{
public:
    explicit MockAlarm(Time::TimeUnit_t<Unit> duration) : Time::Alarm<Unit>(duration) {}
    MOCK_METHOD(bool, isJoinable, (), (const, override));
    MOCK_METHOD(bool, shouldAlarmRing, (), (const, override));
   // MOCK_METHOD(void, stop, (), (override));
};

template <Time::TimeUnit Unit = Time::TimeUnit::SECONDS>
class Task
{
public:
    explicit Task(Time::Alarm<Unit>& alarm) : alarm_(alarm) {}
    void execute()
    {
        while (!alarm_.shouldAlarmRing()){}
    }
private:
    Time::Alarm<Unit>& alarm_;
};
}
#endif // MOCKALARM_H
