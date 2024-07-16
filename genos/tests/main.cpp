#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <igris/time/systime.h>

int64_t system_time()
{
    return igris::millis();
}

float systime_frequency()
{
    return 1000;
}