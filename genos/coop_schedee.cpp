#include <genos/schedee.h>
#include <genos/coop_schedee.h>
#include <genos/schedee_api.h>

#include <stdlib.h>

#include <igris/defs/schedee.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>

static void coop_schedee_starter (void * priv)
{
	genos::coop_schedee * csch = (genos::coop_schedee *) priv;
	void* ret = csch->task(csch->arg);
	csch->ret = ret;
	current_schedee_exit();
}

void genos::coop_schedee::execute()
{
	context_load(&cntxt);
}

int genos::coop_schedee::displace()
{
	//flag.runned = 0;
	context_save_and_displace(&cntxt);
	return SCHEDEE_DISPLACE_REAL;
}

void genos::coop_schedee::finalize()
{
	if (flag.dynamic_heap)
		BUG();
	if (flag.dynamic)
		BUG();
}

genos::coop_schedee::coop_schedee(void* (*task) (void*),
                                  void * arg,
                                  void * heap,
                                  int heapsize,
                                  int flags)
:	schedee(0, SCHEDEE_USE_PARENT_GID)
{
	context_init(
	    &cntxt,
	    (uintptr_t) heap + heapsize - 1,
	    coop_schedee_starter,
	    (void*)this,
	    1);

	this->heap = heap;
	this->heapsize = heapsize;
	this->task = task;
	this->arg = arg;
	this->flag.has_context = 1;
	this->flag.can_displace = 1;
}
