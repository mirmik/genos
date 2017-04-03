#ifndef GENOS_SCHEDEE_MANAGER_H
#define GENOS_SCHEDEE_MANAGER_H

#include <asm/startup.h>
#include <kernel/sched/SchedeeManager.h>

namespace Genos {
	namespace Glue { void core(); }

	void displaceSchedule() {
		while(1) {
			RESET_STACK();
			schedeeManager()->currentSchedee(nullptr);
			core();
		}
	}
}

#endif