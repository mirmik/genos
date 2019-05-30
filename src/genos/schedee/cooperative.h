#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <hal/subst.h>
#include <genos/schedee.h>

class cooperative_schedee : public schedee
{
	struct context cntxt;

	void* (*task) (void*);
	void * arg;
	void * heap;
	size_t heapsize;

	void * ret;

public:
	void init(void* (*task) (void*),
                              void * arg,
                              void * heap,
                              int heapsize);

	void execute() override;
	int displace() override;
	void finalize() override; 

	friend 
	void coopstarter(void * sch);
};

#endif