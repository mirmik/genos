#include <doctest/doctest.h>
#include <ralgo/lp/simplex.h>

TEST_CASE("simplex")
{
    ralgo::simplex simplex = std::vector{
        ralgo::vector{0., 0.},
        ralgo::vector{1., 0.},
        ralgo::vector{0., 1.},
    };

    CHECK_EQ(simplex.baricentric({0, 0}), ralgo::vector<double>{1, 0, 0});
    CHECK_EQ(simplex.baricentric({1, 0}), ralgo::vector<double>{0, 1, 0});
    CHECK_EQ(simplex.baricentric({0, 1}), ralgo::vector<double>{0, 0, 1});
    CHECK_EQ(simplex.baricentric({0, 0.5}), ralgo::vector<double>{0.5, 0, 0.5});
}