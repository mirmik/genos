#include <doctest/doctest.h>
#include <ralgo/linalg/inverse.h>
#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/trivial_inverse.h>

TEST_CASE("trivial_inverse_upper")
{
    ralgo::matrix<double> A{
        {2, 4, 6},
        {0, -1, -8},
        {0, 0, 96},
    };

    ralgo::matrix<double> A_inv = ralgo::U_triangle_inverse(A);

    CHECK_EQ(A_inv,
             ralgo::matrix<double>{
                 {0.5, 2, 13. / 96.},
                 {0, -1, -1. / 12.},
                 {0, 0, 1. / 96},
             });

    CHECK_EQ(ralgo::matops::multiply(A, A_inv), ralgo::matops::eye(A.rows()));
}

TEST_CASE("trivial_inverse_lower")
{
    ralgo::matrix<double> A{
        {1, 0, 0},
        {1, 1, 0},
        {1, 1, 1},
    };

    ralgo::matrix<double> A_inv = ralgo::L_triangle_inverse(A);

    CHECK_EQ(A_inv,
             ralgo::matrix<double>{
                 {1, 0, 0},
                 {-1., 1, 0},
                 {0, -1., 1},
             });

    CHECK_EQ(ralgo::matops::multiply(A, A_inv), ralgo::matops::eye(A.rows()));
}

TEST_CASE("inverse")
{
    ralgo::matrix<double> A{
        {1, 1, 0},
        {1, 0, 1},
        {0, 1, 1},
    };

    ralgo::matrix<double> A_inv = ralgo::inverse(A);

    CHECK_EQ(A_inv,
             ralgo::matrix<double>{
                 {0.5, 0.5, -0.5},
                 {0.5, -0.5, 0.5},
                 {-0.5, 0.5, 0.5},
             });

    CHECK_EQ(ralgo::matops::multiply(A, A_inv), ralgo::matops::eye(A.rows()));
}