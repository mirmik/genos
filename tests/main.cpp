#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <igris/time/ardutime.h>
#include <genos/time.h>

systime_t system_time() 
{
	return millis();
}

float systime_frequency() 
{
	return 1000;
}