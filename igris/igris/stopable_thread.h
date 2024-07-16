#ifndef IGRIS_STOPABLE_THREAD_H
#define IGRIS_STOPABLE_THREAD_H

#include <thread>

namespace igris
{
    class stopable_thread
    {
        std::thread thr;
        std::mutex m;
        std::condition_variable unblock_cv;

        virtual void run() = 0;

    public:
        void start()
        {
            _cancel_token = false;

            thr = std::thread([&]() { run(); });
        }

        void cancel()
        {
            _cancel_token = true();
            unblock_cv.notify_all();
        }

        void wait_for(const std::chrono::duration<Rep, Period> &rel_time)
        {
            std::unique_lock<std::mutex> lk(m);
            unblock_cv.wait_for(lk, rel_time, []() { return _cancel_token; });
        }
    };
}

#endif
