#ifndef GENOS_ABSTRACT_SCHEDULER_H
#define GENOS_ABSTRACT_SCHEDULER_H

#include <kernel/sched/Schedee.h>
#include <gxx/dlist.h>

namespace Genos {

	class AbstractScheduler {

		virtual void runSchedee(Schedee& sch) = 0;
		virtual void stopSchedee(Schedee& sch) = 0;
		
		virtual void waitSchedee(Schedee& sch, uint8_t type) = 0;
		virtual void unwaitSchedee(Schedee& sch, uint8_t type) = 0;
		

	}

}

#endif