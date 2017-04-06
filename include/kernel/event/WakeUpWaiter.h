#ifndef GENOS_WAKE_UP_TASKLET_H
#define GENOS_WAKE_UP_TASKLET_H

#include <kernel/sched/Schedee.h>
#include <kernel/event/Waiter.h>

namespace Genos {

	class WakeUpWaiter : public Waiter {
		Schedee* waitedSchedee;

	public:
		WakeUpWaiter(Schedee* waitedSchedee) : 
			waitedSchedee(waitedSchedee) {}

		void invoke() override { 
			waitedSchedee->unwait();
		}

		Schedee* schedee() {
			return waitedSchedee;
		}
	};

	void wait(WaiterHead& head, WakeUpWaiter& wakeup) {
		wakeup.schedee()->wait();
		head.wait(wakeup);
	}

}

#endif