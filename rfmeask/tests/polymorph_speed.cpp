#include <PolymorphSpeed.h>
#include <doctest/doctest.h>

TEST_CASE("PolymorphSheed")
{
    SUBCASE("external_per_minut")
    {
        auto speed =
            PolymorphSpeed(PositionUnit::EXTERNAL, TimeUnit::MINUTES, 600);

        speed.set_unit2internal_multiplier(100);
        speed.set_rot2internal_multiplier(1);

        CHECK_EQ(speed.unit_per_second(), 10);
        CHECK_EQ(speed.unit_per_minut(), 600);
        CHECK_EQ(speed.rotation_per_second(), 1000);
        CHECK_EQ(speed.rotation_per_minut(), 60000);
    }

    SUBCASE("rotation_per_minut")
    {
        auto speed =
            PolymorphSpeed(PositionUnit::ROTATION, TimeUnit::MINUTES, 600);

        speed.set_unit2internal_multiplier(100);
        speed.set_rot2internal_multiplier(1);

        CHECK_EQ(speed.unit_per_second(), 0.1);
        CHECK_EQ(speed.unit_per_minut(), 6);
        CHECK_EQ(speed.rotation_per_second(), 10);
        CHECK_EQ(speed.rotation_per_minut(), 600);
    }

    SUBCASE("external_per_second")
    {
        auto speed =
            PolymorphSpeed(PositionUnit::EXTERNAL, TimeUnit::SECONDS, 10);

        speed.set_unit2internal_multiplier(100);
        speed.set_rot2internal_multiplier(1);

        CHECK_EQ(speed.unit_per_second(), 10);
        CHECK_EQ(speed.unit_per_minut(), 600);
        CHECK_EQ(speed.rotation_per_second(), 1000);
        CHECK_EQ(speed.rotation_per_minut(), 60000);
    }

    SUBCASE("rotation_per_minut")
    {
        auto speed =
            PolymorphSpeed(PositionUnit::ROTATION, TimeUnit::SECONDS, 10);

        speed.set_unit2internal_multiplier(100);
        speed.set_rot2internal_multiplier(1);

        CHECK_EQ(speed.unit_per_second(), 0.1);
        CHECK_EQ(speed.unit_per_minut(), 6);
        CHECK_EQ(speed.rotation_per_second(), 10);
        CHECK_EQ(speed.rotation_per_minut(), 600);
    }
}