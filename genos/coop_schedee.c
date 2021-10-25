#include <genos/sched.h>
#include <genos/schedee/coop.h>
#include <genos/api.h>

#include <stdlib.h>

#include <igris/defs/schedee.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>

void coop_schedee_starter (void * sch)
{
	struct genos::coop_schedee * csch = (struct genos::coop_schedee *) sch;

	void* ret = csch->task(csch->arg);
	csch->ret = ret;

	schedee_exit();
}

void coop_schedee_execute()
{
	context_load(&cntxt);
}

int coop_schedee_displace()
{
	flag.runned = 0;
	context_save_and_displace(&cntxt);
	return SCHEDEE_DISPLACE_REAL;
}

void coop_schedee_finalize()
{
	if (flag.dynamic_heap)
		BUG();
	//free(asch->heap);

	if (flag.dynamic)
		BUG();
	//free(asch);
}

static const struct schedee_operations operations = 
{
	.execute = coop_schedee_execute,
	.displace = coop_schedee_displace,
	.finalize = coop_schedee_finalize
}

void coop_schedee_init(void* (*task) (void*),
                       void * arg,
                       void * heap,
                       int heapsize,
                       int flags)
{
	schedee_init(
	    this,
	    0,
	    SCHEDEE_USE_PARENT_GID,
	    &operations);

	context_init(
	    &cntxt,
	    (uintptr_t) heap + heapsize - 1,
	    starter,
	    this,
	    1);

	this -> heap = heap;
	this -> heapsize = heapsize;
	this -> task = task;
	this -> arg = arg;
	this -> flag.has_context = 1;
	this -> flag.can_displace = 1;
}
