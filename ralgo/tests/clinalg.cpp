#include <doctest/doctest.h>
#include <ralgo/clinalg/matops.h>

TEST_CASE("clinalg.matops_square_inverse_f")
{
    float a[9] = {2, 0, 0, 0, 4, 0, 0, 0, 8};
    float b[9];

    matops_square_inverse_f(a, 3, 3, b);

    CHECK_EQ(b[0], 0.5);
    CHECK_EQ(b[1], 0);
    CHECK_EQ(b[2], 0);
    CHECK_EQ(b[3], 0);
    CHECK_EQ(b[4], 0.25);
    CHECK_EQ(b[5], 0);
    CHECK_EQ(b[6], 0);
    CHECK_EQ(b[7], 0);
    CHECK_EQ(b[8], 0.125);
}

TEST_CASE("clinalg.matops_square_inverse_d")
{
    double a[9] = {2, 0, 0, 0, 4, 0, 0, 0, 8};
    double b[9];

    matops_square_inverse_d(a, 3, 3, b);

    CHECK_EQ(b[0], 0.5);
    CHECK_EQ(b[1], 0);
    CHECK_EQ(b[2], 0);
    CHECK_EQ(b[3], 0);
    CHECK_EQ(b[4], 0.25);
    CHECK_EQ(b[5], 0);
    CHECK_EQ(b[6], 0);
    CHECK_EQ(b[7], 0);
    CHECK_EQ(b[8], 0.125);
}

TEST_CASE("clinalg.matops_multiply_frr")
{
    float a[4] = {1, 0, 0, 2};
    float b[4] = {2, 0, 0, 2};
    float c[4];

    matops_multiply_frrr(a, 2, 2, 2, b, c);

    CHECK_EQ(c[0], 2);
    CHECK_EQ(c[1], 0);
    CHECK_EQ(c[2], 0);
    CHECK_EQ(c[3], 4);
}
