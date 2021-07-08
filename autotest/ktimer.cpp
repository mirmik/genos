#include <doctest/doctest.h>
#include <genos/ktimer.h>

static int a = 0;

void action(void * privdata, ktimer_t * tim) 
{
	a = 1;
}

TEST_CASE("ktimer invoke") 
{
	ktimer_t tim;
	ktimer_init(&tim, action, NULL, 0, 100);

	CHECK(!ktimer_check(&tim, 0));
	CHECK(ktimer_check(&tim, 100));

	ktimer_plan(&tim);
	CHECK_EQ(a, 0);

	ktimer_manager_step(100);
	CHECK_EQ(a, 1);
}