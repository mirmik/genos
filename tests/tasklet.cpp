#include <doctest/doctest.h>
#include <genos/tasklet.h>

static int a = 0;

static inline 
void action(void * privdata) 
{
	a+=(uintptr_t)privdata;
}

TEST_CASE("tasklet invoke") 
{
	struct tasklet task0;
	struct tasklet task1;

	tasklet_init(&task0, action, (void*)1);
	tasklet_init(&task1, action, (void*)2);

	tasklet_plan(&task0);
	tasklet_plan(&task1);

	tasklet_manager_step();
	CHECK_EQ(a, 3);
	
	tasklet_manager_step();
	CHECK_EQ(a, 6);

	tasklet_unplan(&task0);
	tasklet_unplan(&task1);

	tasklet_manager_step();
	CHECK_EQ(a, 6);
}