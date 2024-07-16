#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix_view.h>

#include <main.h>
#include <vector>

using namespace ralgo;

TEST_CASE("matops_test")
{
    double a[9];
    double b[9];
    ralgo::matrix_view<double> amat(a, 3, 3);
    ralgo::matrix_view<double> bmat(b, 3, 3);

    SUBCASE("1")
    {
        CHECK_EQ(amat.size1(), 3);
        CHECK_EQ(amat.size2(), 3);
    }

    SUBCASE("2")
    {
        matops::fill(amat, 22);

        CHECK_EQ(amat(0, 0), 22);
        CHECK_EQ(amat(2, 2), 22);
        CHECK_EQ(amat(2, 1), 22);
    }

    SUBCASE("3")
    {
        std::vector<double> v1[] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        matops::copy_from_rows(bmat, std::begin(v1), std::end(v1));

        CHECK_EQ(bmat(0, 0), 1);
        CHECK_EQ(bmat(1, 1), 5);
        CHECK_EQ(bmat(1, 0), 4);
    }

    SUBCASE("4")
    {
        std::vector<double> v2[] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        matops::copy_from_cols(bmat, std::begin(v2), std::end(v2));

        CHECK_EQ(bmat(0, 0), 1);
        CHECK_EQ(bmat(1, 1), 5);
        CHECK_EQ(bmat(0, 1), 4);
    }

    SUBCASE("5")
    {
        double a[] = {1, 0, 0, 1};
        double b[] = {1, -1, 0, 1};
        double result_buf[4];
        ralgo::matrix_view<double> matrix(a, 2, 2);
        ralgo::matrix_view<double> result(result_buf, 0, 0);
        ralgo::matops::square_matrix_inverse(matrix, result);

        CHECK_EQ(result(0, 0), 1);
        CHECK_EQ(result(0, 1), 0);
        CHECK_EQ(result(1, 1), 1);
        CHECK_EQ(result(1, 0), 0);

        matrix = {b, 2, 2};
        ralgo::matops::square_matrix_inverse(matrix, result);
        CHECK_EQ(result(0, 0), 1);
        CHECK_EQ(result(0, 1), 1);
        CHECK_EQ(result(1, 1), 1);
        CHECK_EQ(result(1, 0), 0);
    }

    SUBCASE("matrix_view_co assign matrix_view_ro")
    {
        double a[4] = {1, 2, 3, 4};
        double b[4];
        ralgo::matrix_view_co<double> matrix_a(a, 2, 2);
        ralgo::matrix_view_ro<double> matrix_b(b, 2, 2);

        ralgo::matops::assign(matrix_a, matrix_b);

        CHECK_EQ(b[0], 1);
        CHECK_EQ(b[1], 3);
        CHECK_EQ(b[2], 2);
        CHECK_EQ(b[3], 4);
    }

    SUBCASE("matrix_view_ro assign matrix_view_co")
    {
        double a[4] = {1, 2, 3, 4};
        double b[4];
        ralgo::matrix_view_ro<double> matrix_a(a, 2, 2);
        ralgo::matrix_view_co<double> matrix_b(b, 2, 2);

        ralgo::matops::assign(matrix_a, matrix_b);

        CHECK_EQ(b[0], 1);
        CHECK_EQ(b[1], 3);
        CHECK_EQ(b[2], 2);
        CHECK_EQ(b[3], 4);
    }

    SUBCASE("matrix_view_co inline_transpose")
    {
        double a[4] = {1, 2, 3, 4};

        ralgo::matrix_view_ro<double> matrix_a(a, 2, 2);
        ralgo::matops::square_inline_transpose(matrix_a);

        CHECK_EQ(a[0], 1);
        CHECK_EQ(a[1], 3);
        CHECK_EQ(a[2], 2);
        CHECK_EQ(a[3], 4);
    }
}
