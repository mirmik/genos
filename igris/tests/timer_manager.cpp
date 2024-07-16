#include <doctest/doctest.h>
#include <igris/time/timer_manager.h>

igris::timer<int> *timptr;
static int aaa = 0;
static int bbb = 0;

static void inc_a(int a, igris::timer_head *tim)
{
    aaa += a;
}

static void inc_b(int b)
{
    bbb += b;

    if (bbb >= 14 * 4)
        timptr->unplan();
}

TEST_CASE("TimerManager")
{
    auto current_time = igris::millis();
    igris::timer_manager manager;
    igris::timer<int> tim1(igris::make_delegate(inc_b), 14);
    igris::timer<int, igris::timer_head *> tim0(
        igris::make_delegate(inc_a), 12, &tim0);
    timptr = &tim1;

    manager.plan(tim0, current_time, 1000);
    manager.plan(tim1, current_time, 2000);

    manager.exec(current_time + 1001);
    CHECK_EQ(aaa, 12);
    CHECK_EQ(bbb, 0);

    manager.exec(current_time + 2001);
    CHECK_EQ(aaa, 24);
    CHECK_EQ(bbb, 14);

    manager.exec(current_time + 3001);
    CHECK_EQ(aaa, 36);
    CHECK_EQ(bbb, 14);
    tim0.unplan();

    manager.exec(current_time + 4001);
    CHECK_EQ(aaa, 36);
    CHECK_EQ(bbb, 28);

    manager.exec(current_time + 6001);
    CHECK_EQ(aaa, 36);
    CHECK_EQ(bbb, 14 * 3);

    tim0.set_start(current_time + 6001);
    manager.plan(tim0);

    manager.exec(current_time + 8001);
    CHECK_EQ(aaa, 12 * 5);
    CHECK_EQ(bbb, 14 * 4);

    manager.exec(current_time + 10001);
    CHECK_EQ(aaa, 12 * 7);
    CHECK_EQ(bbb, 14 * 4);
}