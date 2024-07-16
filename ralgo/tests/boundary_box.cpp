#include <doctest/doctest.h>
#include <ralgo/boundary_box.h>

TEST_CASE("boundary_box 1 dim")
{
    ralgo::boundary_box<double> box(
        {-std::numeric_limits<double>::infinity(), 2, 3}, {3, 4, 5});

    CHECK_EQ(box.mins_as_vector(),
             ralgo::vector<double>(
                 {-std::numeric_limits<double>::infinity(), 2, 3}));
    CHECK_EQ(box.maxs_as_vector(), ralgo::vector<double>({3, 4, 5}));
}

TEST_CASE("boundary_box")
{
    ralgo::boundary_box<double> box({1, 2, 3}, {3, 4, 5});

    ralgo::vector<double> pnt({2, 3, 4});

    auto coeffs = box.lerpcoeffs(pnt);

    CHECK_EQ(coeffs[0], 0.5);
    CHECK_EQ(coeffs[1], 0.5);
    CHECK_EQ(coeffs[2], 0.5);

    CHECK_EQ(box.mins_as_vector(), ralgo::vector<double>({1, 2, 3}));
    CHECK_EQ(box.maxs_as_vector(), ralgo::vector<double>({3, 4, 5}));
}

TEST_CASE("boundary_box max equal min")
{
    ralgo::boundary_box<double> box({1, 2, 3}, {1, 2, 3});

    {
        ralgo::vector<double> pnt({1, 2, 3});
        auto coeffs = box.lerpcoeffs(pnt);
        CHECK_EQ(coeffs[0], 0);
        CHECK_EQ(coeffs[1], 0);
        CHECK_EQ(coeffs[2], 0);
    }

    {
        ralgo::vector<double> pnt({-1, 22, 33});
        auto coeffs = box.lerpcoeffs(pnt);
        CHECK_EQ(coeffs[0], 0);
        CHECK_EQ(coeffs[1], 0);
        CHECK_EQ(coeffs[2], 0);
    }

    CHECK_EQ(box.mins_as_vector(), ralgo::vector<double>({1, 2, 3}));
    CHECK_EQ(box.maxs_as_vector(), ralgo::vector<double>({1, 2, 3}));
}