#include <Axis.h>
#include <doctest/doctest.h>
#include <ndmath/util.h>

bool near(double a, double b, double eps = 1e-7)
{
    return std::abs(a - b) < eps;
}

TEST_CASE("oneaxis_correction_table_file")
{
    // Axis axis("TestAx", nullptr);

    auto correction = one_axis_cartesian_correction::correction_from_csv_file(
        "tests/correction.csv");

    // axis.set_one_axis_correction(correction, true);

    // nos::println(correction.deltas());

    auto grid = correction.grid();
    auto zone = grid.cellzone({0});
    CHECK_EQ(zone.maxs_as_vector()[0], -10);
    CHECK_EQ(zone.mins_as_vector()[0],
             -std::numeric_limits<double>::infinity());

    CHECK_EQ(correction.corrected_point({160}), ndmath::point{160 + 0.026151});
    CHECK_EQ(correction.corrected_point({150}),
             ndmath::point{150 - (149.969938 - 150)});
    CHECK_EQ(correction.corrected_point({-10}),
             ndmath::point{-10 - (-9.994084 + 10)});

    /*
    0  160,159.973849
    1  150,149.969938
    2  140,139.960810
    3  130,129.962761
    4  120,119.966537
    5  110,109.970623
    6  100,99.976728
    7  90,89.979797
    8  80,79.984123
    9  70,69.988752
    10 60,59.993601
    11 50,49.998379
    12 40,39.989212
    13 30,29.990190
    14 20,19.994083
    15 10,9.997365
    16 0,0.000000
    17 -10,-9.994084
*/
}
