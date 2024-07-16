#include <doctest/doctest.h>

#include <igris/math.h>
#include <ralgo/trajectory/tsdeform.h>

TEST_CASE("tsdeformer")
{
    trajectory_speed_deformer deform;

    tsdeform_set_timestamp_pattern(&deform, 0.2, 0.2, 0, 0);
    CHECK(igris::early(deform.v1, 1.25));
    CHECK(igris::early(deform.t3, 1));
    CHECK_EQ(tsdeform_posmod(&deform, 1), 1);
    CHECK_EQ(tsdeform_spdmod(&deform, 0.5), deform.v1);

    tsdeform_set_speed_pattern(&deform, 0.2, 0.2, 0, 0, 0);
    CHECK(igris::early(deform.v1, 1));
    CHECK(igris::early(deform.t3, 1.2));
    CHECK_EQ(tsdeform_posmod(&deform, deform.t3), 1);
    CHECK_LT(tsdeform_posmod(&deform, 1), 1);
    CHECK_EQ(tsdeform_spdmod(&deform, 0.5), 1);
}

TEST_CASE("tsdeformer2")
{
    trajectory_speed_deformer deform;

    tsdeform_set_speed_pattern(&deform, 0.25, 0.25, 0, 0, true);
    CHECK(igris::early(deform.x0h, 0.25 / 12));
    CHECK(igris::early(tsdeform_posmod(&deform, 0.125),
                       2 * (0.125 * 0.125 * 0.125) / 3 / (0.25 * 0.25)));
    CHECK(igris::early(tsdeform_spdmod(&deform, 0.125),
                       2 * (0.125 * 0.125) / (0.25 * 0.25)));
}
