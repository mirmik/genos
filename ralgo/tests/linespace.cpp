#include <doctest/doctest.h>
#include <ralgo/linspace.h>

TEST_CASE("linspace")
{
    auto l = ralgo::linspace(0, 2, 0, true);
    CHECK_EQ(l.size(), 2);
    CHECK_EQ(l[0], 0);
    CHECK_EQ(l[1], 2);
}