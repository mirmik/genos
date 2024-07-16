#include <AbstractAxis.h>
#include <Axis.h>
#include <doctest/doctest.h>
#include <ndmath/util.h>
#include <nos/print.h>

TEST_CASE("oneaxis_correction_table_save")
{
    Axis axis("TestAx", nullptr);

    auto correction = one_axis_cartesian_correction::arrays_to_correction(
        {0, 1, 2}, {0, 0.1, 0.2});

    axis.set_one_axis_correction(correction, true);

    auto tr = axis.one_axis_correction_as_trent();

    nos::println(tr);

    CHECK(tr.contains("points"));
    CHECK(tr.contains("deltas"));
    CHECK(tr.contains("enabled"));
}

TEST_CASE("oneaxis_correction_table_save.not_sorted")
{
    Axis axis("TestAx", nullptr);

    auto correction = one_axis_cartesian_correction::arrays_to_correction(
        {1, 0, 2}, {0.1, 0, 0.2});

    axis.set_one_axis_correction(correction, true);

    auto tr = axis.one_axis_correction_as_trent();
    nos::println(tr);

    CHECK_EQ(correction.grid().cellzone({0}).maxs_as_vector()[0], 0);
    CHECK_EQ(correction.grid().cellzone({0}).mins_as_vector()[0],
             -std::numeric_limits<double>::infinity());
    CHECK_EQ(correction.grid().cellzone({1}).maxs_as_vector()[0], 1);
    CHECK_EQ(correction.grid().cellzone({1}).mins_as_vector()[0], 0);
    CHECK_EQ(correction.grid().cellzone({2}).maxs_as_vector()[0], 2);
    CHECK_EQ(correction.grid().cellzone({2}).mins_as_vector()[0], 1);
    CHECK_EQ(correction.grid().cellzone({3}).maxs_as_vector()[0],
             std::numeric_limits<double>::infinity());
    CHECK_EQ(correction.grid().cellzone({3}).mins_as_vector()[0], 2);

    CHECK(tr.contains("points"));
    CHECK(tr.contains("deltas"));
    CHECK(tr.contains("enabled"));
}
