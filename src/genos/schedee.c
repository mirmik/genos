#include "schedee.h"

void schedee_init(schedee_t * sch, 
	const struct schedee_operations * ops,
	schedee_t * parent) 
{
	sch.ops = ops;
	sch.parent = parent;

	sch.prio = SCHEDEE_MAXPRIO;
	sch.state = SCHEDEE_STATE_STOP;

	ctrobj_init(&sch->ctr);
}
