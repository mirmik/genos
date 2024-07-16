#include <PolymorphAccel.h>
#include <doctest/doctest.h>

TEST_CASE("PolymorphAccel")
{
    SUBCASE("external_per_minut")
    {
        auto speed =
            PolymorphAccel(PositionUnit::EXTERNAL, TimeUnit::MINUTES, 36000);

        speed.set_unit2internal_multiplier(100);

        // CHECK_EQ(speed.unit_per_secondsqr(), 10);
        // CHECK_EQ(speed.unit_per_minut(), 36000);
        // CHECK_EQ(speed.internal_per_second(), 1000);
        // CHECK_EQ(speed.internal_per_minut(), 3600000);
    }
}