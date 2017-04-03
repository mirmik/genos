#ifndef GENOS_TASKLET_MANAGER_H
#define GENOS_TASKLET_MANAGER_H

#include <kernel/event/Tasklet.h>
#include <kernel/sched/AutomSchedee.h>

namespace Genos {

	class TaskletManagerSchedee : public AutomFunctorSchedee {
		gxx::dlist<Tasklet, &Tasklet::lnk> queue;

	public:
		TaskletManagerSchedee() : AutomFunctorSchedee(0) {}

		void addToExecute(Tasklet& slot) {
			critical_section_enter();
			queue.move_back(slot);
			run();
			critical_section_leave();
		};

		void routine() {
			while(!queue.empty()) {
				critical_section_enter();
				Tasklet& object = *queue.begin();
				queue.del_init(object);
				critical_section_leave();
				object.routine();
			}
			stop();
		}
	};
}

#endif