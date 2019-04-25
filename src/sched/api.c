#include <sched/api.h>

void schedee_exit()
{
	struct schedee * sch;

	sch = current_schedee();
	schedee_final(sch);

	__displace__();
}
