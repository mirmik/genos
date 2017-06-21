#ifndef GENOS_STANDART_SCHEDULER_H
#define GENOS_STANDART_SCHEDULER_H

//#include <kernel/sched/AbstractScheduler.h>
#include <sched/Schedee.h>

namespace genos {
	/*class Scheduler : public AbstractScheduler {

		virtual void displace() = 0;
		virtual void schedule() = 0;
	}*/

	namespace sched {
		static constexpr uint8_t substtype = 0;
		static constexpr uint8_t automtype = 1;

		void schedule();
		void displace(uint8_t schedeeType);
	}

	
}

#endif