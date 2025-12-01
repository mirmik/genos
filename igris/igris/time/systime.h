/**@file*/

#ifndef IGRIS_SYSTIME_H
#define IGRIS_SYSTIME_H

#include <stdint.h>

namespace igris
{
    int64_t system_time();
    static inline int64_t systime()
    {
        return system_time();
    }
    uint32_t systime_frequency();
    uint64_t systime_millis();

    static inline int64_t ms2systime(float ms)
    {
        return static_cast<int64_t>(ms * (systime_frequency() * 0.001f));
    }

    static inline int64_t us2systime(float us)
    {
        return static_cast<int64_t>(us * (systime_frequency() * 0.000001f));
    }

    static inline int64_t ns2systime(double ns)
    {
        return static_cast<int64_t>(ns * (systime_frequency() * 0.000000001));
    }

    int64_t millis();
    int64_t micros();
    int64_t nanos();

    void delay(int64_t ms);
}

#endif
