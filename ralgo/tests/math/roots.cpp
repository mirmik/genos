#include <doctest/doctest.h>
#include <ralgo/linalg/roots.h>
#include <ralgo/linalg/vector.h>
#include <ralgo/poly/poly.h>

TEST_CASE("roots.1")
{
    ralgo::vector<double> coeffs{-1, 1};
    auto roots = ralgo::roots(coeffs);
    CHECK_EQ(roots, ralgo::vector<double>{1});
}

TEST_CASE("roots.2")
{
    ralgo::vector<double> coeffs{1, 2, 1};
    auto roots = ralgo::roots(coeffs);
    CHECK_EQ(roots, ralgo::vector<double>{-1, -1});
}

TEST_CASE("roots.3")
{
    ralgo::vector<double> coeffs{1, 3, 3, 1};
    auto roots = ralgo::roots(coeffs);
    CHECK_EQ(roots[0], doctest::Approx(-1.));
}

TEST_CASE("roots.3.2")
{
    auto p = ralgo::poly<double>{2, 1} * ralgo::poly<double>{-4, 1} *
             ralgo::poly<double>{6, 1};

    auto roots = ralgo::roots(p.coeffs);
    CHECK_EQ(roots, ralgo::vector<double>{-2, 4, -6});
}

TEST_CASE("roots.3.3")
{
    auto p = ralgo::poly<double>{2, 1} * ralgo::poly<double>{2, 1} *
             ralgo::poly<double>{2, 1};

    auto roots = ralgo::roots(p.coeffs);
    CHECK_EQ(roots, ralgo::vector<double>{-2, -2, -2});
}