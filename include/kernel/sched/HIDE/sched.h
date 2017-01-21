#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

#include <kernel/sched/schedee.h>

#define PRIORITY_TOTAL 5
#define ERRSCHEDBLOCK -2

namespace Kernel {

	void sched_init();
	
	int schedule();

	void currentSchedee(schedee*);
	schedee* currentSchedee();

	void set_ready(schedee* sch, int8_t opt);
//	void set_held(schedee* sch);
	void set_block(schedee* sch, uint16_t state);
	void set_zombie(schedee* sch);

}

#endif