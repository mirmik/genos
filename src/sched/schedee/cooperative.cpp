#include <sched/sched.h>
#include <sched/api.h>
#include <stdlib.h>

//#include <gxx/panic.h>
#include <hal/subst.h>

#include <igris/dprint.h>

struct cooperative_schedee
{
	struct schedee sch;

	struct context cntxt;

	void* (*task) (void*);
	void * arg;
	void * heap;
	size_t heapsize;

	void * ret;

	//void* (*task) (void*);
	//void * arg;
};

static void cooperative_schedee_execute(struct schedee* sch);
static void cooperative_schedee_displace(struct schedee* sch);
static void cooperative_schedee_finalize(struct schedee* sch);

struct schedee_operations cooperative_schedee_op =
{
	.execute = cooperative_schedee_execute,
	.displace = cooperative_schedee_displace,
	.finalize = cooperative_schedee_finalize,
};

static inline void starter (void * sch)
{
	//TRACE();
	struct cooperative_schedee * csch = (struct cooperative_schedee *) sch;

	void* ret = csch->task(csch->arg);
	csch->ret = ret;

	schedee_exit();
}

struct schedee * create_cooperative_schedee(void* (*task) (void*), void * arg, int heapsize)
{
	//TRACE();
	struct cooperative_schedee * sch = (struct cooperative_schedee *)
	                                   malloc(sizeof(struct cooperative_schedee));

	void* heap = malloc(heapsize);

	schedee_init(&sch->sch, 0, &cooperative_schedee_op);
	context_init(&sch->cntxt, (uintptr_t) heap + heapsize - 1, starter, sch, 1);

	sch -> heap = heap;
	sch -> heapsize = heapsize;
	sch -> task = task;
	sch -> arg = arg;
	sch -> sch.flag.has_context = 1;
	sch -> sch.flag.can_displace = 1;

	return &sch->sch;
}

static void cooperative_schedee_execute(struct schedee* sch)
{
	//TRACE();
	struct cooperative_schedee * csch = mcast_out(sch, struct cooperative_schedee, sch);
	context_load(&csch->cntxt);
}

static void cooperative_schedee_displace(struct schedee* sch)
{
	//TRACE();
	struct cooperative_schedee * csch = mcast_out(sch, struct cooperative_schedee, sch);
	sch->flag.runned = 0;
	context_save_and_invoke_displace(&csch->cntxt);
}

static void cooperative_schedee_finalize(struct schedee* sch)
{
	//TRACE();
	struct cooperative_schedee * asch = mcast_out(sch, struct cooperative_schedee, sch);
	free(asch);
}