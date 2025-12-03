#include <igris/time/systime.h>
#include <nos/check.h>
#include <onboardtest/test.h>

TEST_CASE("stm32 systime")
{
    auto start = igris::millis();
    igris::delay(10);
    auto end = igris::millis();
    NOS_CHECK_EQ(end - start, 10);
}