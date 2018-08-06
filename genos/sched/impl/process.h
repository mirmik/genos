#ifndef GENOS_PROCESS_IMPL_H
#define GENOS_PROCESS_IMPL_H

#include <genos/sched/schedee.h>
#include <arch/context.h>
#include <arch/startup.h>
#include <gxx/buffer.h>

namespace genos {
	struct process_schedee : public schedee {
		gxx::buffer zone;	
		context cntxt;	
		
		void(*func)(void*);
		void* arg;

		void execute() override;
		void finalize() override;		
		void displace() override;
	};
}

#endif