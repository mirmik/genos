#include <doctest/doctest.h>
#include <ralgo/geom/simplex/triangle2d.h>
#include <ralgo/geom/simplex/triangle3d.h>

TEST_CASE("geom/simpex.cpp")
{
    ralgo::geom::simplex::triangle2d<double> tri{{0, 0}, {1, 0}, {0, 1}};

    CHECK_EQ(tri.baricentric_of_point2d({0, 0}),
             linalg::vec<double, 3>{1, 0, 0});
    CHECK_EQ(tri.baricentric_of_point2d({0, 1}),
             linalg::vec<double, 3>{0, 0, 1});
    CHECK_EQ(tri.baricentric_of_point2d({1, 0}),
             linalg::vec<double, 3>{0, 1, 0});
}

TEST_CASE("is ray intersects triangle")
{
    ralgo::geom::simplex::triangle3d<double> tri{
        {0, 0, 0}, {1, 0, 0}, {0, 1, 0}};

    auto s = 0.5;

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{0.1, 0.1, 1},
                 ralgo::geom::point3d<double>{0.1, 0.1, -1})),
             true);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{0.1, -0.1, 1},
                 ralgo::geom::point3d<double>{0.1, -0.1, -1})),
             false);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{-0.1, -0.1, 1},
                 ralgo::geom::point3d<double>{-0.1, -0.1, -1})),
             false);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{s + 0.01, s, 1},
                 ralgo::geom::point3d<double>{s, s, -1})),
             false);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{s - 0.01, s, 1},
                 ralgo::geom::point3d<double>{s, s, -1})),
             true);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{s, s + 0.01, 1},
                 ralgo::geom::point3d<double>{s, s, -1})),
             false);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{s, s - 0.01, 1},
                 ralgo::geom::point3d<double>{s, s, -1})),
             true);

    CHECK_EQ(tri.is_intersect(ralgo::geompar::ray3d<double>::from_points(
                 ralgo::geom::point3d<double>{s - 1, s - 0.01, 1},
                 ralgo::geom::point3d<double>{s + 1, s, -1})),
             true);
}