#include <sched/sched.h>
#include <arch/startup.h>

extern "C" int __kill_stack_and_schedule_invoke__()
{
	TRACE();
	RESET_STACK();

	while (1) __schedule__();
}