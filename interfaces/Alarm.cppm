//
// Created by Sourav.Bhattacharjee on 21.08.2025.
//
module;
#include <mutex>
#include <thread>
export module alarm;
import timebase;

export namespace Time
{
    template<TimeUnit unit = TimeUnit::SECONDS>
    class Alarm
    {
    public:
        //Alarm() = default;
        explicit Alarm(TimeUnit_t<unit> duration)
            : finished_(false), stop_(false), duration_(duration)
        {
            alarmThread_ = std::thread([this]()
            {
                std::unique_lock<std::mutex> lock(mutex_);
                [[maybe_unused]] bool timeUp = condition_.wait_for(lock, duration_, [this]()
                                                                   { return stop_.load(); });
                if (!stop_.load())
                    finished_.store(true);
            });
        }
        Alarm(const Alarm&) = delete;
        Alarm& operator=(const Alarm&) = delete;

        Alarm(Alarm&& other) noexcept
            : alarmThread_(std::move(other.alarmThread_))
            , finished_(other.finished_.load())
            , stop_(other.stop_.load())
            , duration_(other.duration_){}

        Alarm& operator=(Alarm&& other) noexcept
        {
            if (this != &other)
            {
                if (alarmThread_.joinable())
                {
                    stop();
                    alarmThread_.join();
                }
            }

            alarmThread_ = std::move(other.alarmThread_);
            duration_ = other.duration_;
            stop_.store(other.stop_.load());
            finished_.store(other.finished_.load());
            return *this;
        }


        virtual ~Alarm()
        {
            if (alarmThread_.joinable())
            {
                stop();
                alarmThread_.join();
            }
        }

        void stop()
        {
            stop_.store(true);
            condition_.notify_all();
        }

        virtual bool shouldAlarmRing() const { return finished_.load(); };
        virtual bool isJoinable() const { return alarmThread_.joinable(); }
    private:
        std::thread alarmThread_;
        std::atomic<bool> finished_;
        std::atomic<bool> stop_;
        std::mutex mutex_;
        std::condition_variable condition_;
        TimeUnit_t<unit> duration_;
    };
}
