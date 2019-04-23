#include <sched/sched.h>
#include <igris/dprint.h>
#include <asm/startup.h>

/*extern "C" int __kill_stack_and_schedule_invoke__()
{
	TRACE();
	RESET_STACK();

	while (1) __schedule__();
}*/

extern "C" int __context_drop__()
{
	dprln("__context_drop__");
	RESET_STACK();
	while (1) __schedule__();
}