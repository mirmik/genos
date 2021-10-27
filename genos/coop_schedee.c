#include <genos/schedee.h>
#include <genos/coop_schedee.h>
#include <genos/schedee_api.h>

#include <stdlib.h>

#include <igris/defs/schedee.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>

void coop_schedee_starter (void * priv)
{
	struct coop_schedee * csch = (struct coop_schedee *) priv;
	void* ret = csch->task(csch->arg);
	csch->ret = ret;
	current_schedee_exit();
}

void coop_schedee_execute(struct schedee * sch)
{
	struct coop_schedee * csch = mcast_out(sch, struct coop_schedee, sch);
	context_load(&csch->cntxt);
}

int coop_schedee_displace(struct schedee * sch)
{
	struct coop_schedee * csch = mcast_out(sch, struct coop_schedee, sch);
	sch->flag.runned = 0;
	context_save_and_displace(&csch->cntxt);
	return SCHEDEE_DISPLACE_REAL;
}

void coop_schedee_finalize(struct schedee * sch)
{
	struct coop_schedee * csch = mcast_out(sch, struct coop_schedee, sch);
	if (sch->flag.dynamic_heap)
		BUG();
	if (sch->flag.dynamic)
		BUG();
}

static const struct schedee_operations operations = 
{
	.execute = coop_schedee_execute,
	.displace = coop_schedee_displace,
	.finalize = coop_schedee_finalize
};

void coop_schedee_init(struct coop_schedee * csch,
					   void* (*task) (void*),
                       void * arg,
                       void * heap,
                       int heapsize,
                       int flags)
{
	schedee_init(
	    &csch->sch,
	    0,
	    SCHEDEE_USE_PARENT_GID,
	    &operations);

	context_init(
	    &csch->cntxt,
	    (uintptr_t) heap + heapsize - 1,
	    coop_schedee_starter,
	    csch,
	    1);

	csch->heap = heap;
	csch->heapsize = heapsize;
	csch->task = task;
	csch->arg = arg;
	csch->sch.flag.has_context = 1;
	csch->sch.flag.can_displace = 1;
}
