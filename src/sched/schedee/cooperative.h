#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <hal/subst.h>
#include <sched/schedee.h>

struct cooperative_schedee
{
	struct schedee sch;
	struct context cntxt;

	void* (*task) (void*);
	void * arg;
	void * heap;
	size_t heapsize;

	void * ret;
};

extern const struct schedee_operations cooperative_schedee_op;

__BEGIN_DECLS
void cooperative_schedee_init(struct cooperative_schedee *,
                              void* (*task) (void*),
                              void * arg,
                              void * heap,
                              int heapsize);
__END_DECLS

#endif