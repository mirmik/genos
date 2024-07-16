#include <doctest/doctest.h>
#include <ralgo/linspace.h>

#include <nos/print.h>
#include <ralgo/linalg/vector.h>
#include <vector>

TEST_CASE("linspace")
{
    CHECK_EQ(ralgo::lerp(1., 4., 0.5), 2.5);
    CHECK_EQ(ralgo::lerp(1., 4., 0.), 1);
    CHECK_EQ(ralgo::lerp(1., 4., 1.f), 4);

    auto ls = ralgo::linspace<double>(1, 4, 4);
    std::vector<double> vec(ls.begin(), ls.end());
    CHECK_EQ(ls[0], doctest::Approx(1));
    CHECK_EQ(ls[1], doctest::Approx(2));
    CHECK_EQ(ls[2], doctest::Approx(3));
    CHECK_EQ(ls[3], doctest::Approx(4));

    auto ls2 = ralgo::linspace<double>(1, 6, 5, false);
    std::vector<double> vec2(ls2.begin(), ls2.end());
    CHECK_EQ(ls2[0], doctest::Approx(1));
    CHECK_EQ(ls2[1], doctest::Approx(2));
    CHECK_EQ(ls2[2], doctest::Approx(3));
    CHECK_EQ(ls2[3], doctest::Approx(4));
    CHECK_EQ(ls2[4], doctest::Approx(5));
}

TEST_CASE("bilinear")
{
    CHECK_EQ(ralgo::bilinear_interpolation(0.f, 0.f, 1.f, 2.f, 3.f, 4.f), 1.f);
    CHECK_EQ(ralgo::bilinear_interpolation(1.f, 0.f, 1.f, 2.f, 3.f, 4.f), 2.f);
    CHECK_EQ(ralgo::bilinear_interpolation(0.f, 1.f, 1.f, 2.f, 3.f, 4.f), 3.f);
    CHECK_EQ(ralgo::bilinear_interpolation(1.f, 1.f, 1.f, 2.f, 3.f, 4.f), 4.f);
}

TEST_CASE("vector linspace")
{
    auto ls =
        ralgo::linspace(ralgo::vector{1., 1.5}, ralgo::vector{4., 4.5}, 4);
    CHECK_EQ(ls[0], ralgo::vector{1., 1.5});
    CHECK_EQ(ls[1], ralgo::vector{2., 2.5});
    CHECK_EQ(ls[2], ralgo::vector{3., 3.5});
    CHECK_EQ(ls[3], ralgo::vector{4., 4.5});
}