#include <doctest/doctest.h>
#include <genos/ktimer.h>

static int a = 0;

void action(void * privdata, genos::ktimer * tim) 
{
	a = 1;
}

TEST_CASE("ktimer invoke") 
{
	genos::ktimer tim(action, NULL, 0, 100);

	CHECK(!tim.check(0));
	CHECK(tim.check(100));

	tim.plan();
	CHECK_EQ(a, 0);

	genos::ktimer_manager_step();
	CHECK_EQ(a, 1);

	tim.deinit();
}