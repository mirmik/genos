#ifndef IGRIS_STOPABLE_THREAD_H
#define IGRIS_STOPABLE_THREAD_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace igris
{
    class stopable_thread
    {
        std::thread thr;
        std::mutex m;
        std::condition_variable unblock_cv;
        std::atomic<bool> cancel_token{false};

    protected:
        virtual void run() = 0;

    public:
        virtual ~stopable_thread()
        {
            cancel();
            if (thr.joinable())
                thr.join();
        }

        void start()
        {
            cancel_token.store(false, std::memory_order_relaxed);
            thr = std::thread([this]() { run(); });
        }

        void cancel()
        {
            cancel_token.store(true, std::memory_order_relaxed);
            unblock_cv.notify_all();
        }

        template <class Rep, class Period>
        void wait_for(const std::chrono::duration<Rep, Period> &rel_time)
        {
            std::unique_lock<std::mutex> lk(m);
            unblock_cv.wait_for(
                lk, rel_time, [this]() { return cancel_token.load(); });
        }

        bool cancelled() const
        {
            return cancel_token.load(std::memory_order_relaxed);
        }
    };
}

#endif
