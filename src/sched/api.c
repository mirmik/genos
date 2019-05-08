#include <sched/api.h>
#include <igris/dprint.h>

void schedee_exit()
{
	struct schedee * sch;

	dprln("schedee_exit");

	sch = current_schedee();
	schedee_final(sch);

	__displace__();

	dprln("schedee_exit after displace!!!");
}
