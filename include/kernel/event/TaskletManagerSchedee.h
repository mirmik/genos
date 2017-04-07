#ifndef GENOS_TASKLET_MANAGER_SCHEDEE_H
#define GENOS_TASKLET_MANAGER_SCHEDEE_H

#include <kernel/event/Tasklet.h>
#include <kernel/sched/AutomSchedee.h>
#include <kernel/csection.h>

namespace Genos {

	class TaskletManagerSchedee : public AutomFunctorSchedee {
		WaiterList queue;

	public:
		TaskletManagerSchedee() {}

		void addToExecute(Tasklet& slot) {
			critical_section_enter();
			queue.move_back(slot);
			run();
			critical_section_leave();
		};

		void routine() override {
			while(!queue.empty()) {
				critical_section_enter();
				Tasklet& object = reinterpret_cast<Tasklet&>(*queue.begin());
				queue.del_init(object);
				critical_section_leave();
				object.routine();
			}
			stop();
		}
	};
}

#endif