#include <doctest/doctest.h>
#include <igris/object_time.h>

TEST_CASE("object_time")
{
    igris::object_time objtime;

    CHECK_EQ(objtime.ToBroken(0), 0);
    CHECK_EQ(objtime.ToBroken(20), 20);

    objtime.SetReferencePoint(40, false);
    CHECK_EQ(objtime.ToBroken(0), 0);
    CHECK_EQ(objtime.ToBroken(20), 20);
    CHECK_EQ(objtime.ToBroken(50), 50);
    CHECK_EQ(objtime.ToBroken(60), 60);
    CHECK_EQ(objtime.BrokenToTimeline(60), 60);

    objtime.SetReferencePoint(60, true);
    CHECK_EQ(objtime.ToBroken(60), 60);
    CHECK_EQ(objtime.ToBroken(50), 70);
    CHECK_EQ(objtime.ToBroken(59), 61);
    CHECK_EQ(objtime.BrokenToTimeline(70), 50);

    objtime.SetReferencePoint(30, false);
    CHECK_EQ(objtime.ToBroken(40), 100);
    CHECK_EQ(objtime.BrokenToTimeline(100), 40);
}