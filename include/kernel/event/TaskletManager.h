#ifndef GENOS_TASKLET_MANAGER_H
#define GENOS_TASKLET_MANAGER_H

#include <kernel/event/Tasklet.h>

namespace Genos {

	class TaskletManager {
		gxx::dlist<Tasklet, &Tasklet::lnk> queue;

	public:
		void addToExecute(Tasklet& slot) {
			queue.move_back(slot);
		};

		void execute() {
			//dprln("TaskletManager::execute");
			while(!queue.empty()) {
				Tasklet& object = *queue.begin();
				queue.del_init(object);
				object.routine();
			}
		}
	};
}

#endif