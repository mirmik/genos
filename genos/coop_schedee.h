#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <genos/schedee.h>
#include <zillot/context.h>
#include <asm/context.h>

namespace genos
{
	struct coop_schedee : public schedee
	{
		struct context cntxt;

		void* (*task) (void*);
		void * arg;
		void * heap;
		size_t heapsize;

		void * ret;

	public:
		coop_schedee(void* (*task) (void*),
		             void * arg,
		             void * heap,
		             int heapsize);
		coop_schedee() = default;

		void init(void* (*task) (void*),
		             void * arg,
		             void * heap,
		             int heapsize);
		
		void execute() override;
		void finalize() override;
		int displace() override;
	};
}

#endif