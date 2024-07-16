#include <doctest/doctest.h>
#include <ralgo/clinalg/solve.h>
#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/solve.h>

#include <ralgo/linalg/trivial_solve.h>

#include <memory>

// x = A*b

TEST_CASE("pivot_solve")
{
    ralgo::matrix<double> mat{
        {0, 0, 1},
        {1, 0, 0},
        {0, 1, 0},
    };

    CHECK_EQ(pivot_solve(mat, ralgo::vector<double>{3, 1, 2}),
             ralgo::vector<double>{1, 2, 3});
}

TEST_CASE("L_triangle_solve")
{
    ralgo::matrix<double> mat{
        {1, 0, 0},
        {1, 1, 0},
        {1, 1, 1},
    };

    CHECK_EQ(L_triangle_solve(mat, ralgo::vector<double>{1, 2, 3}),
             ralgo::vector<double>{1, 1, 1});

    ralgo::matrix<double> mat1{
        {2, 0, 0},
        {3, 5, 0},
        {4, 2, 3},
    };

    CHECK_EQ(L_triangle_solve(mat1, ralgo::vector<double>{2, 13, 11}),
             ralgo::vector<double>{1, 2, 1});
}

TEST_CASE("U_triangle_solve")
{
    ralgo::matrix<double> mat{
        {1, 1, 1},
        {0, 1, 1},
        {0, 0, 1},
    };

    CHECK_EQ(U_triangle_solve(mat, ralgo::vector<double>{3, 2, 1}),
             ralgo::vector<double>{1, 1, 1});
}

TEST_CASE("solve")
{
    ralgo::matrix<float> A;
    ralgo::vector<float> x, b;

    A = ralgo::matops::diag({1.f, 1.f, 1.f});
    x = ralgo::vector{1.f, 1.f, 1.f};
    b = ralgo::solve(A, x);
    CHECK_EQ(b[0], 1);
    CHECK_EQ(b[1], 1);
    CHECK_EQ(b[2], 1);

    A = ralgo::matops::diag({2.f, 1.f, 2.f});
    x = ralgo::vector{1.f, 1.f, 1.f};
    b = ralgo::solve(A, x);
    CHECK_EQ(b[0], 0.5);
    CHECK_EQ(b[1], 1);
    CHECK_EQ(b[2], 0.5);
}

TEST_CASE("linalg_square_solve")
{
    double A[] = {1, -1, 0, 0.5};
    double b[] = {1, 1};
    double x[2];

    linalg_square_solve_d(A, 2, b, x);

    CHECK_EQ(x[0], 3);
    CHECK_EQ(x[1], 2);
}

TEST_CASE("linalg_square_solve_2")
{
    double A[] = {0, 2, 0, 0, 0, 2, 1, 1, 1};
    double b[] = {0.5, 0.5, 1};
    double x[3];

    linalg_square_solve_d(A, 3, b, x);

    CHECK_EQ(x[0], 0.5);
    CHECK_EQ(x[1], 0.25);
    CHECK_EQ(x[2], 0.25);
}
