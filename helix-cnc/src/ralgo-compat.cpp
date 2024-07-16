#include <igris/time/systime.h>
#include <ralgo/disctime.h>

disctime_t discrete_time()
{
    return igris::millis();
}

float discrete_time_frequency()
{
    return 1000;
}