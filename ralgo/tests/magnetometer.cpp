#include <doctest/doctest.h>
#include <ralgo/calibration/magnetometer.h>

TEST_CASE("magnetometer")
{
    int yaw_total = 6;
    int pitch_total = 6;

    linalg::vec<float, 3> array[yaw_total * (pitch_total - 2) + 2];

    auto collector =
        ralgo::spherical_cloud_collector(yaw_total, pitch_total, array);

    CHECK_EQ(collector.yaw_by_index(1), doctest::Approx(M_PI / 3));
    CHECK_EQ(collector.yaw_by_index(5), doctest::Approx(M_PI * 5 / 3));

    CHECK_EQ(collector.pitch_by_index(0), doctest::Approx(-M_PI / 2));
    CHECK_EQ(collector.pitch_by_index(1),
             doctest::Approx(-M_PI / 2 + M_PI / 5));
    CHECK_EQ(collector.pitch_by_index(5), doctest::Approx(M_PI / 2));

    CHECK_EQ(collector.angles_by_index(0)[1], collector.pitch_by_index(0));
    CHECK_EQ(collector.angles_by_index(0)[0], 0);

    CHECK_EQ(collector.angles_by_index(2)[1], collector.pitch_by_index(1));
    CHECK_EQ(collector.angles_by_index(2)[0], collector.yaw_by_index(1));

    CHECK_EQ(collector.angles_by_index(8)[1], collector.pitch_by_index(2));
    CHECK_EQ(collector.angles_by_index(8)[0], collector.yaw_by_index(1));

    CHECK_EQ(collector.angles_by_index(25)[1], collector.pitch_by_index(5));
    CHECK_EQ(collector.angles_by_index(25)[0], 0);

    CHECK_EQ(collector.angles_by_index(24)[1], collector.pitch_by_index(4));
    CHECK_EQ(collector.angles_by_index(24)[0], collector.yaw_by_index(5));

    CHECK_EQ(collector.index_by_angles({0, -M_PI / 2}), 0);
    CHECK_EQ(collector.index_by_angles({M_PI + 0.3, -M_PI / 2 + 0.001}), 0);
    CHECK_EQ(collector.index_by_angles({0.7, M_PI / 2 - 0.001}),
             yaw_total * (pitch_total - 2) + 1);

    CHECK_EQ(collector.index_by_angles({0, -M_PI / 2 + M_PI / 5}), 1);
    CHECK_EQ(collector.index_by_angles({M_PI, -M_PI / 2 + M_PI / 5}), 4);
    CHECK_EQ(collector.index_by_angles({M_PI, 0.001}), 4 + 6 * 2);

    CHECK_EQ(collector.index_by_angles({2 * M_PI - 0.01, -M_PI / 2 + M_PI / 5}),
             6);

    CHECK_EQ(collector.index_by_angles({M_PI, M_PI / 2 - M_PI / 5}), 26 - 4);
}
