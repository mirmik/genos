#include <igris/time/systime.h>

#include <chrono>
#include <thread>

int64_t igris::millis()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

int64_t igris::micros()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto mcross =
        std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return mcross;
}

int64_t igris::nanos()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto ncross =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return ncross;
}

void igris::delay(int64_t m)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(m));
}

int64_t igris::system_time() 
{
    return millis();
}