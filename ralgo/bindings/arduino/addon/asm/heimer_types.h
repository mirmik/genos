#ifndef RALGO_HEIMER_TYPES_H
#define RALGO_HEIMER_TYPES_H

#include <Arduino.h>

typedef unsigned long disctime_t;

static inline unsigned long discrete_time()
{
    return millis();
}

static inline float discrete_time_frequency()
{
    return 1000.f;
}

#endif
