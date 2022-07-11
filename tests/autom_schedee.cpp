#include <doctest/doctest.h>

#include <genos/schedee.h>
#include <genos/autom_schedee.h>

#include <genos/schedee_api.h>
#include <nos/print.h>

static int a = 0;

void foo(void * priv, int * state) 
{
	switch (*state) 
	{
		case 0: a = 1; ++(*state); return;
		case 1: a = 2; ++(*state); return;
		case 2: a = 3; ++(*state); return;
	}
}


void bar(void * priv, int * state) 
{
	switch (*state) 
	{
		case 0: a = 1; ++(*state); genos::current_schedee_msleep(10, 0); return;
		case 1: a = 2; ++(*state); return;
		case 2: a = 3; ++(*state); return;
		case 3: a = 4; ++(*state); genos::current_schedee_msleep(20, 20); return;
		case 4: a = 5; ++(*state); return;
		case 5: a = 6; ++(*state); return;
	}
}



TEST_CASE("aschedee") 
{
	genos::schedee_manager_init();

	genos::autom_schedee sch(foo, nullptr);

	genos::schedee_start(&sch);

	genos::schedee_manager_step();
	CHECK_EQ(a,1);

	genos::schedee_manager_step();
	CHECK_EQ(a,2);

	genos::schedee_manager_step();
	CHECK_EQ(a,3);

	genos::schedee_deinit(&sch);
}

TEST_CASE("aschedee_with_timer") 
{
	genos::schedee_manager_init();

	genos::autom_schedee sch(bar, nullptr);

	genos::schedee_start(&sch);

	genos::schedee_manager_step();
	CHECK_EQ(a, 1);

	genos::schedee_manager_step();
	CHECK_EQ(a, 1);

	genos::schedee_manager_step();
	CHECK_EQ(a, 1);

	CHECK_EQ(genos::ktimer_manager_planed_count(), 1);
	CHECK_EQ(sch.ktimer.interval, 10);
	CHECK_EQ(sch.ktimer.start, 0);

	genos::ktimer_manager_step(20);
	CHECK_EQ(genos::ktimer_manager_planed_count(), 0);

	genos::schedee_manager_step();
	CHECK_EQ(a, 2);

	genos::schedee_manager_step();
	CHECK_EQ(a, 3);

	genos::ktimer_manager_step(20);
	genos::schedee_manager_step();
	CHECK_EQ(a, 4);

	genos::ktimer_manager_step(30);
	genos::schedee_manager_step();
	CHECK_EQ(a, 4);

	genos::ktimer_manager_step(40);
	genos::schedee_manager_step();
	CHECK_EQ(a, 5);

	genos::schedee_manager_step();
	CHECK_EQ(a, 6);

	genos::schedee_deinit(&sch);
}
