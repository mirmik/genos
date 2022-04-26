#include <doctest/doctest.h>
#include <genos/schedee.h>
#include <nos/print.h>

static int a = 0;
static int b = 0;

void execute0(struct schedee * sch) 
{
	a++;
}

void execute1(struct schedee * sch) 
{
	b++;
}

void finalize(struct schedee * sch) 
{
	nos::println("finalize");
}

const schedee_operations ops0 = {
	.execute = execute0,
	.finalize = finalize
};

const schedee_operations ops1 = {
	.execute = execute1,
	.finalize = finalize
};

TEST_CASE("schedee") 
{
	struct schedee sch0;
	struct schedee sch1;

	schedee_manager_init();

	schedee_init(&sch0, 0, 0, &ops0); 	
	schedee_start(&sch0);

	schedee_manager_step();

	CHECK_EQ(a, 1);

	schedee_init(&sch1, 0, 0, &ops1); 	
	schedee_start(&sch1);

	schedee_manager_step();

	CHECK_EQ(a, 2);
	CHECK_EQ(b, 0);

	schedee_manager_step();

	CHECK_EQ(a, 2);
	CHECK_EQ(b, 1);

	schedee_deinit(&sch0);
}
