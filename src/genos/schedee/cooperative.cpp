#include <genos/sched.h>
#include <genos/schedee/cooperative.h>
#include <genos/api.h>
#include <stdlib.h>

#include <igris/util/bug.h>
#include <igris/dprint.h>

void coopstarter (void * sch)
{
	struct cooperative_schedee * csch = (struct cooperative_schedee *) sch;

	void* ret = csch->task(csch->arg);
	csch->ret = ret;

	schedee_exit();
}

void cooperative_schedee::init(void* (*task) (void*),
                              void * arg,
                              void * heap,
                              int heapsize)
{
	schedee_init(this, 0);
	context_init(&cntxt, (uintptr_t) heap + heapsize - 1, coopstarter, this, 1);

	this -> heap = heap;
	this -> heapsize = heapsize;
	this -> task = task;
	this -> arg = arg;
	this -> flag.has_context = 1;
	this -> flag.can_displace = 1;
}

/*struct schedee * create_cooperative_schedee(void* (*task) (void*),
        void * arg,
        int heapsize)
{
	DTRACE();
	struct cooperative_schedee * sch = (struct cooperative_schedee *)
	                                   malloc(sizeof(struct cooperative_schedee));
	
	char* heap = (char*) malloc(heapsize);

	cooperative_schedee_init(sch, task, arg, heap, heapsize);

	sch->sch.flag.dynamic = true;
	sch->sch.flag.dynamic_heap = true;

	return &sch->sch;
}*/

void cooperative_schedee::execute()
{
	context_load(&cntxt);
}

int cooperative_schedee::displace()
{
	flag.runned = 0;
	context_save_and_displace(&cntxt);
	return DISPLACE_REAL;
}

void cooperative_schedee::finalize()
{
	if (flag.dynamic_heap)
		BUG();
		//free(asch->heap);

	if (flag.dynamic)
		BUG();
		//free(asch);
}