#ifndef MEASURE_UNITS_H
#define MEASURE_UNITS_H

#include <stdexcept>

enum PositionUnit
{
    EXTERNAL,
    ROTATION,
    INTERNALPULSE
};

enum TimeUnit
{
    SECONDS,
    MINUTES
};

static inline double timeunit_to_seconds_multiplier(TimeUnit unit)
{
    switch (unit)
    {
    case TimeUnit::SECONDS:
        return 1.;
    case TimeUnit::MINUTES:
        return 60.;
    }
    throw std::runtime_error("unreacheble code");
}

static inline double seconds_to_timeunit_multiplier(TimeUnit unit)
{
    return 1 / timeunit_to_seconds_multiplier(unit);
}

#endif