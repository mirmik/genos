#include <doctest/doctest.h>
#include <genos/autom_schedee.h>
#include <genos/resource/namespace.h>
#include <genos/resource/mvfs.h>

void schedee_and_files_foo(void * priv, int * state) 
{
	switch(*state) 
	{
		case 0: mvfs_open("/dev/null", 0); break;
		case 1: break;
		case 2: break;
	}

	(*state)++;
}

TEST_CASE("schedee_and_files") 
{
	struct autom_schedee asch;
	schedee_manager_init();

	autom_schedee_init(&asch, schedee_and_files_foo, NULL);


	schedee_start(&asch.sch);



	schedee_manager_step();


	schedee_deinit(&asch.sch);
}
