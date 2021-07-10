#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <hal/subst.h>
#include <genos/schedee.h>

namespace genos
{

	class coop_schedee : public schedee
	{
		struct context cntxt;

		void* (*task) (void*);
		void * arg;
		void * heap;
		size_t heapsize;

		void * ret;

	public:

		coop_schedee(){}

		coop_schedee(void* (*task) (void*),
		          void * arg,
		          void * heap,
		          int heapsize,
		          int flags=0) 
		{
			init(task, arg, heap, heapsize, flags);
		}

		void init(void* (*task) (void*),
		          void * arg,
		          void * heap,
		          int heapsize,
		          int flags=0);

		void execute() override;
		int displace() override;
		void finalize() override;

		static void starter(void * sch);
	};

	using coopschedee = coop_schedee;
}

using coop_schedee = genos::coop_schedee;

#define COOPSCHEDEE_DECLARE(name, task, arg, heapsize) 			\
	uint8_t name##_heap[heapsize];								\
	genos::coopschedee name(task, arg, name##_heap, heapsize);

#endif