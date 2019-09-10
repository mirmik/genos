#include <genos/sched.h>
#include <genos/schedee/coop.h>
#include <genos/api.h>
#include <stdlib.h>

#include <igris/defs/schedee.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>

void genos::coopschedee::starter (void * sch)
{
	struct genos::coopschedee * csch = (struct genos::coopschedee *) sch;

	void* ret = csch->task(csch->arg);
	csch->ret = ret;

	schedee_exit();
}

void genos::coopschedee::init(void* (*task) (void*),
                              void * arg,
                              void * heap,
                              int heapsize)
{
	schedee_init(this, 0);
	context_init(&cntxt, (uintptr_t) heap + heapsize - 1, starter, this, 1);

	this -> heap = heap;
	this -> heapsize = heapsize;
	this -> task = task;
	this -> arg = arg;
	this -> flag.has_context = 1;
	this -> flag.can_displace = 1;
}

/*struct schedee * create_genos::coopschedee(void* (*task) (void*),
        void * arg,
        int heapsize)
{
	DTRACE();
	struct genos::coopschedee * sch = (struct genos::coopschedee *)
	                                   malloc(sizeof(struct genos::coopschedee));
	
	char* heap = (char*) malloc(heapsize);

	genos::coopschedee_init(sch, task, arg, heap, heapsize);

	sch->sch.flag.dynamic = true;
	sch->sch.flag.dynamic_heap = true;

	return &sch->sch;
}*/

void genos::coopschedee::execute()
{
	context_load(&cntxt);
}

int genos::coopschedee::displace()
{
	flag.runned = 0;
	context_save_and_displace(&cntxt);
	return SCHEDEE_DISPLACE_REAL;
}

void genos::coopschedee::finalize()
{
	if (flag.dynamic_heap)
		BUG();
		//free(asch->heap);

	if (flag.dynamic)
		BUG();
		//free(asch);
}