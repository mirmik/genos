#include <doctest/doctest.h>
#include <ralgo/lp/point_in_hexagon.h>

TEST_CASE("point_in_simplex")
{
    double A[] = {0, 0, 2, 0, 0, 2};

    double target0[] = {0.5, 0.5};
    CHECK_EQ(point_in_simplex_d(A, 2, target0), 1);

    double target1[] = {-0.1, 0.5};
    CHECK_EQ(point_in_simplex_d(A, 2, target1), 0);

    double target2[] = {0.5, 2.1};
    CHECK_EQ(point_in_simplex_d(A, 2, target2), 0);

    double target3[] = {1, 1};
    CHECK_EQ(point_in_simplex_d(A, 2, target3), 1);

    double target4[] = {0, 0};
    CHECK_EQ(point_in_simplex_d(A, 2, target4), 1);
}

TEST_CASE("point_in_hexagon")
{
    double A[] = {0, 0, 2, 0, 0, 2, 2, 2};

    double target0[] = {1, 1};
    CHECK_EQ(point_in_hexagon_d(A, 2, 4, target0), 1);

    double target1[] = {3, 1};
    CHECK_EQ(point_in_hexagon_d(A, 2, 4, target1), 0);

    double target2[] = {0.5, 1};
    CHECK_EQ(point_in_hexagon_d(A, 2, 4, target2), 1);

    double target3[] = {1.5, 1.3};
    CHECK_EQ(point_in_hexagon_d(A, 2, 4, target3), 1);

    double target4[] = {-0.1, 1};
    CHECK_EQ(point_in_hexagon_d(A, 2, 4, target4), 0);

    double target5[] = {1, 2.1};
    CHECK_EQ(point_in_hexagon_d(A, 2, 4, target5), 0);
}
