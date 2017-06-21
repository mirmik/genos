#ifndef GENOS_STANDART_SCHEDULER_H
#define GENOS_STANDART_SCHEDULER_H

//#include <kernel/sched/AbstractScheduler.h>
#include <kernel/sched/Schedee.h>

namespace Genos {
	/*class Scheduler : public AbstractScheduler {

		virtual void displace() = 0;
		virtual void schedule() = 0;
	}*/

	namespace Glue {
		static constexpr uint8_t SubstitutionSchedeeType = 0;
		static constexpr uint8_t AutomateSchedeeType = 1;

		void schedule();
		void displace(uint8_t schedeeType);
	}

	
}

#endif