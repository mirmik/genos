#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <genos/schedee.h>
#include <asm/context.h>

class coop_schedee
{
	struct schedee sch;
	struct context cntxt;

	void* (*task) (void*);
	void * arg;
	void * heap;
	size_t heapsize;

	void * ret;
};

__BEGIN_DECLS

void coop_schedee_init(void* (*task) (void*),
                       void * arg,
                       void * heap,
                       int heapsize,
                       int flags);

__END_DECLS

#endif