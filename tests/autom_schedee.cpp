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
		case 0: a = 1; ++(*state); current_schedee_msleep(10); return;
		case 1: a = 2; ++(*state); return;
		case 2: a = 3; ++(*state); return;
	}
}



TEST_CASE("aschedee") 
{
	schedee_manager_init();

	autom_schedee sch;
	autom_schedee_init(&sch, foo, nullptr);

	schedee_start(&sch.sch);

	schedee_manager_step();
	CHECK_EQ(a,1);

	schedee_manager_step();
	CHECK_EQ(a,2);

	schedee_manager_step();
	CHECK_EQ(a,3);

	schedee_deinit(&sch.sch);
}

TEST_CASE("aschedee_with_timer") 
{
	schedee_manager_init();
	ktimer_manager_init();

	autom_schedee sch;
	autom_schedee_init(&sch, bar, nullptr);

	schedee_start(&sch.sch);

	schedee_manager_step();
	CHECK_EQ(a, 1);

	schedee_manager_step();
	CHECK_EQ(a, 1);

	schedee_manager_step();
	CHECK_EQ(a, 1);

	CHECK_EQ(ktimer_manager_planed_count(), 1);

	ktimer_manager_step(systime() + ms2systime(11));
	CHECK_EQ(ktimer_manager_planed_count(), 0);

	schedee_manager_step();
	CHECK_EQ(a, 2);

	schedee_manager_step();
	CHECK_EQ(a, 3);

	schedee_deinit(&sch.sch);
}
