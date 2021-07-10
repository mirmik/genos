#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <igris/systime.h>
#include <genos/time.h>

int64_t system_time() 
{
	return millis();
}

float systime_frequency() 
{
	return 1000;
}