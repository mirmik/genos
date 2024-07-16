#include <doctest/doctest.h>
#include <ralgo/poly/poly.h>

TEST_CASE("poly.subs")
{
    ralgo::poly<double> p{1, 2, 3};
    CHECK_EQ(p(0), 1);
}

TEST_CASE("poly.derivative")
{
    ralgo::poly<double> p{1, 2, 3};
    CHECK_EQ(p.derivative(), ralgo::poly<double>{2, 6});
}

TEST_CASE("poly.integral")
{
    ralgo::poly<double> p{2, 6};
    CHECK_EQ(p.integral(), ralgo::poly<double>{0, 2, 3});
}

TEST_CASE("poly.eval")
{
    ralgo::poly<double> p{1, 2, 3};
    CHECK_EQ(p(1), 6);
}

TEST_CASE("poly.multiply")
{
    ralgo::poly<double> p{1, 2, 3};
    ralgo::poly<double> q{1, 2};
    CHECK_EQ(p * q, ralgo::poly<double>{1, 2 + 2, 3 + 4, 6});
}

TEST_CASE("poly.multiply2")
{
    ralgo::poly<double> p{1, 2, 3};
    ralgo::poly<double> q{1, 2, 3};
    CHECK_EQ(
        p * q,
        ralgo::poly<double>{
            1 * 1, 1 * 2 + 2 * 1, 1 * 3 + 2 * 2 + 3 * 1, 2 * 3 + 3 * 2, 3 * 3});
}

TEST_CASE("poly.divide")
{
    ralgo::poly<double> p{4, 4, 7, 6};
    ralgo::poly<double> q{1, 2};
    CHECK_EQ(p.div(q).first, ralgo::poly<double>{1, 2, 3});
    CHECK_EQ(p.div(q).second, ralgo::poly<double>{3});
}

TEST_CASE("poly.gcd")
{
    ralgo::poly<double> p{4, 4, 7, 6};
    ralgo::poly<double> q{1, 2};
    CHECK_EQ(p.gcd(q), ralgo::poly<double>{1, 2});
}

TEST_CASE("poly.gcd2")
{
    ralgo::poly<double> p{4, 4, 7, 6};
    ralgo::poly<double> q{1, 2, 3};
    CHECK_EQ(q.gcd(p), ralgo::poly<double>{1, 2, 3});
}

TEST_CASE("poly.gcd3")
{
    ralgo::poly<double> p{-3, -4, -1, 3, 1};
    ralgo::poly<double> q{-3, 2, 10, 3};

    CHECK_EQ(p.div(q.gcd(p)).second.coeffs[0], doctest::Approx(0));
    CHECK_EQ(q.div(q.gcd(p)).second.coeffs[0], doctest::Approx(0));
    CHECK_EQ(p.div(q.gcd(p)).second.degree(), 0);
    CHECK_EQ(q.div(q.gcd(p)).second.degree(), 0);
}