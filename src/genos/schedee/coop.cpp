#include <genos/sched.h>
#include <genos/schedee/coop.h>
#include <genos/api.h>
#include <stdlib.h>

#include <igris/defs/schedee.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>

void genos::coop_schedee::starter (void * sch)
{
	struct genos::coop_schedee * csch = (struct genos::coop_schedee *) sch;

	void* ret = csch->task(csch->arg);
	csch->ret = ret;

	schedee_exit();
}

void genos::coop_schedee::init(void* (*task) (void*),
                              void * arg,
                              void * heap,
                              int heapsize, 
                              int flags)
{
	schedee_init(
		this, 
		0,
		SCHEDEE_USE_PARENT_GID);
	
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

void genos::coop_schedee::execute()
{
	context_load(&cntxt);
}

int genos::coop_schedee::displace()
{
	flag.runned = 0;
	context_save_and_displace(&cntxt);
	return SCHEDEE_DISPLACE_REAL;
}

void genos::coop_schedee::finalize()
{
	if (flag.dynamic_heap)
		BUG();
		//free(asch->heap);

	if (flag.dynamic)
		BUG();
		//free(asch);
}