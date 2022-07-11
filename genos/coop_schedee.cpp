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
	genos::current_schedee_exit();
}

void genos::coop_schedee::execute()
{
	context_load(&cntxt);
}

int genos::coop_schedee::displace()
{
	context_save_and_displace(&cntxt);
	return SCHEDEE_DISPLACE_REAL;
}

void genos::coop_schedee::finalize()
{
}

genos::coop_schedee::coop_schedee(void* (*task) (void*),
                                  void * arg,
                                  void * heap,
                                  int heapsize)
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
	this->u.f.has_context = 1;
	this->u.f.can_displace = 1;
}

void genos::coop_schedee::init(void* (*task) (void*),
                                  void * arg,
                                  void * heap,
                                  int heapsize)
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
	this->u.f.has_context = 1;
	this->u.f.can_displace = 1;
}
