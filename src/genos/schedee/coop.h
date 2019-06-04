#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <hal/subst.h>
#include <genos/schedee.h>

namespace genos
{

	class coopschedee : public schedee
	{
		struct context cntxt;

		void* (*task) (void*);
		void * arg;
		void * heap;
		size_t heapsize;

		void * ret;

	public:
		coopschedee(void* (*task) (void*),
		          void * arg,
		          void * heap,
		          int heapsize) 
		{
			init(task, arg, heap, heapsize);
		}

		void init(void* (*task) (void*),
		          void * arg,
		          void * heap,
		          int heapsize);

		void execute() override;
		int displace() override;
		void finalize() override;

		static void starter(void * sch);
	};

}

#define COOPSCHEDEE_DECLARE(name, task, arg, heapsize) 			\
	uint8_t name##_heap[heapsize];								\
	genos::coopschedee name(task, arg, name##_heap, heapsize);

#endif