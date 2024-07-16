#include <doctest/doctest.h>
#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/vector.h>

TEST_CASE("multiply")
{
    ralgo::matrix<double> m{{2, 3}, {1, 1}};

    ralgo::vector<double> v = {1, 2};

    auto sqr_m = ralgo::matops::multiply(m, m);
    CHECK_EQ(sqr_m, ralgo::matrix<double>{{7, 9}, {3, 4}});

    CHECK_EQ(ralgo::matops::multiply(m, v), ralgo::vector<double>{8, 3});
}
