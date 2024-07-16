#include <doctest/doctest.h>
#include <ralgo/space/pose3.h>

TEST_CASE("pose3")
{
    ralgo::pose3<double> a{{0, 0, 0, 1}, {10, 0, 0}};
    ralgo::pose3<double> b{{0, 0, 0, 1}, {10, 0, 0}};
    ralgo::pose3<double> c{{0, 0, 0, 1}, {20, 0, 0}};

    CHECK_EQ(a * b, c);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
TEST_CASE("euler")
{
    ralgo::pose3<double> c{{0, 0, 0, 1}, {0, 0, 0}};

    CHECK_EQ(ralgo::pose3<double>::euler_rotation({0, 0, 0}), c);
}
#pragma GCC diagnostic pop

TEST_CASE("translation")
{
    ralgo::pose3<double> c{{0, 0, 0, 1}, {10, 0, 0}};
    CHECK_EQ(ralgo::pose3<double>::translation({10, 0, 0}), c);
}
