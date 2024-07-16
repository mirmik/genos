#include <doctest/doctest.h>
#include <ralgo/lp/point_relations.h>

TEST_CASE("angle over")
{
    linalg::vec<float, 2> a = {1, -1};
    linalg::vec<float, 2> b = {1, 1};
    linalg::vec<float, 2> c = {0, 0};

    auto angle = line_over_point2_argument(a, b, c);

    CHECK_EQ(angle, doctest::Approx(90. / 180. * M_PI));
}

TEST_CASE("angle over polyline")
{
    linalg::vec<float, 2> polyline[] = {
        {1, 1},
        {-1, 1},
        {-1, -1},
        {1, -1},
    };
    linalg::vec<float, 2> c = {0, 0};

    auto angle = polyline_over_point2_argument(polyline, 4, c);

    CHECK_EQ(angle, doctest::Approx(270. / 180. * M_PI));
}

TEST_CASE("angle over polyline 2")
{
    linalg::vec<float, 2> polyline[] = {
        {1, 1},
        {-1, 1},
        {-1, -1},
        {1, -1},
    };
    linalg::vec<float, 2> c = {0, 0};

    auto angle = polyline_over_point2_argument(polyline, 4, c, true);

    CHECK_EQ(angle, doctest::Approx(360. / 180. * M_PI));
}

TEST_CASE("angle over polyline 2")
{

    linalg::vec<float, 2> polyline[] = {
        {0, 0},
        {0, 10},
        {10, 10},
        {10, 20},
        {20, 20},
        {20, 10},
        {30, 10},
        {30, 0},
    };

    CHECK_EQ(polyline_over_point2_argument(polyline, 8, {10, 5}, true),
             doctest::Approx(-360. / 180. * M_PI));

    CHECK_EQ(polyline_over_point2_argument(polyline, 8, {15, 15}, true),
             doctest::Approx(-360. / 180. * M_PI));

    CHECK_EQ(polyline_over_point2_argument(polyline, 8, {5, 15}, true),
             doctest::Approx(0. / 180. * M_PI));
}

TEST_CASE("angle over polyline 3")
{

    linalg::vec<float, 2> polyline[] = {
        {0, 0},
        {0, 10},
        {10, 10},
        {10, 20},
        {20, 20},
        {20, 10},
        {30, 10},
        {30, 0},
    };

    CHECK_EQ(point2_in_polygon({10, 5}, polyline, 8), true);

    CHECK_EQ(point2_in_polygon({15, 15}, polyline, 8), true);

    CHECK_EQ(point2_in_polygon({5, 15}, polyline, 8), false);
}
