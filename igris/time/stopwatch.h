#ifndef IGRIS_TIME_STOPWATH_H
#define IGRIS_TIME_STOPWATH_H

#include <chrono>

namespace igris
{
    class stopwatch
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> _start = {};
        std::chrono::time_point<std::chrono::high_resolution_clock> _stop = {};

    public:
        stopwatch() {}

        void start()
        {
            _start = std::chrono::high_resolution_clock::now();
        }

        void stop()
        {
            _stop = std::chrono::high_resolution_clock::now();
        }

        double elapsed_ms()
        {
            return static_cast<double>(
                std::chrono::duration_cast<std::chrono::milliseconds>(_stop -
                                                                      _start)
                    .count());
        }

        double elapsed_us()
        {
            return static_cast<double>(
                std::chrono::duration_cast<std::chrono::microseconds>(_stop -
                                                                      _start)
                    .count());
        }

        double elapsed_ns()
        {
            return static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(_stop -
                                                                     _start)
                    .count());
        }
    };
}

#endif