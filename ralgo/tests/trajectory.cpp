#include <doctest/doctest.h>
#include <ralgo/trajectory/LineTrajectory.h>

TEST_CASE("LineTrajectory")
{
    ralgo::LinePositionFunction<double, double> func(1, 2, 1, 2);

    CHECK_EQ(func(2), 2);
    CHECK_EQ(func(1), 1);
    CHECK_EQ(func(1.5), 1.5);
}