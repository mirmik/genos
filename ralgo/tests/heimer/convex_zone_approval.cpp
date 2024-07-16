#include <doctest/doctest.h>
#include <ralgo/heimer/convex_zone_approval.h>

using namespace heimer;

TEST_CASE("convex_zone_approval")
{
    convex_zone_approval cza;
    double table[64];

    cza.init(2);
    cza.bind_table(table, 64, 0);

    CHECK_EQ(cza.room(), 64);

    double points[] = {10, 10, 10, 20, 20, 10, 20, 20};

    cza.extend(points, 4);

    CHECK_EQ(cza.room(), 60);

    double strt[2] = {0, 0};
    double fini[2] = {15, 15};

    CHECK_EQ(cza.check(2, strt, fini), 1);
}
