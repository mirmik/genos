#include <doctest/doctest.h>
#include <ralgo/trajectory/linetraj.h>

TEST_CASE("line trajectory")
{
    line_trajectory ltraj;
    position_t pos;
    velocity_t vel;

    sf_position_t sfpos;
    position_t spos = 300;
    position_t fpos = 400;

    ltraj.init(1, &sfpos, 0);

    ltraj.init_nominal_speed(0, 100, &spos, &fpos, 10, 10, false);

    ltraj.attime(0, &pos, &vel);
    CHECK_EQ(pos, 300);
    CHECK_EQ(vel, 0);

    ltraj.attime(120, &pos, &vel);
    CHECK_EQ(pos, 400);
    CHECK_EQ(vel, 0);

    ltraj.attime(55, &pos, &vel);
    CHECK_EQ(pos, 350);
    CHECK_EQ(vel, 1);
}
