#include <doctest/doctest.h>
#include <nos/print.h>
#include <ralgo/imu/madgwick.h>
#include <ralgo/linalg/linalg.h>

double deg(double x)
{
    return x / 180. * M_PI;
}

using linalg::ostream_overloads::operator<<;

TEST_CASE("madgwick")
{
    ralgo::madgwick madgwick;

    SUBCASE("a")
    {
        madgwick.set_koeffs(100, 10);

        madgwick.reset(linalg::rotation_quat<float>({0., 0., 1.}, 0));
        madgwick.update(0, 0, 0, 0, 0, -1, 1, 0, 0);
        CHECK_EQ(madgwick.hx, doctest::Approx(1).epsilon(0.001));
        CHECK_EQ(madgwick.hy, doctest::Approx(0).epsilon(0.001));

        madgwick.reset(linalg::rotation_quat<float>({0., 0., 1.}, deg(90)));
        madgwick.update(0, 0, 0, 0, 0, -1, 1, 0, 0);
        CHECK_EQ(madgwick.hx, doctest::Approx(0).epsilon(0.001));
        CHECK_EQ(madgwick.hy, doctest::Approx(1).epsilon(0.001));

        madgwick.reset(linalg::rotation_quat<float>({0., 0., 1.}, deg(180)));
        madgwick.update(0, 0, 0, 0, 0, -1, 1, 0, 0);
        CHECK_EQ(madgwick.hx, doctest::Approx(-1).epsilon(0.001));
        CHECK_EQ(madgwick.hy, doctest::Approx(0).epsilon(0.001));

        madgwick.reset(linalg::rotation_quat<float>({0., 0., 1.}, deg(270)));
        madgwick.update(0, 0, 0, 0, 0, -1, 1, 0, 0);
        CHECK_EQ(madgwick.hx, doctest::Approx(0).epsilon(0.001));
        CHECK_EQ(madgwick.hy, doctest::Approx(-1).epsilon(0.001));

        madgwick.reset(linalg::rotation_quat<float>({0., 0., 1.}, deg(90)));
        madgwick.apply(0, 0, 0, 0, 0, -1, 0, 0, 0);

        madgwick.reset(linalg::rotation_quat<float>({0., 0., 1.}, deg(90)));
        madgwick.apply(0, 0, 0, 0, 0, -1);

        for (int i = 0; i < 100; ++i)
        {
            madgwick.update(0, 0, 0, 0, 0, -1, 0, 1, 0);
        }
        CHECK_EQ(madgwick.quat()[3], doctest::Approx(0.70710).epsilon(0.001));
        CHECK_EQ(madgwick.quat()[2], doctest::Approx(0.70710).epsilon(0.001));

        for (int i = 0; i < 100; ++i)
        {
            madgwick.update(0, 0, 0, 0, 0, -1, 1, 0, 0);
        }
        CHECK_EQ(madgwick.quat()[3], doctest::Approx(1).epsilon(0.001));
        CHECK_EQ(madgwick.quat()[2], doctest::Approx(0).epsilon(0.001));

        for (int i = 0; i < 100; ++i)
        {
            madgwick.update(0, 0, 0, 0, 0, -1, 0, -1, 0);
        }
        CHECK_EQ(madgwick.quat()[3], doctest::Approx(0.70710).epsilon(0.001));
        CHECK_EQ(madgwick.quat()[2], doctest::Approx(0.70710).epsilon(0.001));

        for (int i = 0; i < 100; ++i)
        {
            madgwick.update(0, 0, 0, 0, 0, -1, -1, 0, 0);
        }
        CHECK_EQ(madgwick.quat()[3], doctest::Approx(0).epsilon(0.001));
        CHECK_EQ(madgwick.quat()[2], doctest::Approx(1).epsilon(0.001));
    }

    SUBCASE("horizon")
    {
        madgwick.reset(linalg::vec<float, 4>{0, 0, 0, 1});
        CHECK_EQ(madgwick.horizon_projection(linalg::vec<float, 3>{8, 9, 10}),
                 linalg::vec<float, 3>{8, 9, 0});

        madgwick.reset(linalg::rotation_quat<float>({0., 1., 0.}, M_PI / 4));
        CHECK(linalg::length(
                  madgwick.horizon_projection(linalg::vec<float, 3>{1, 0, 0}) -
                  linalg::vec<float, 3>{0.5, 0, 0.5}) < 1e-7);
    }
}
