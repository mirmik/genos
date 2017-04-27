#ifndef GENOS_WAKE_UP_TASKLET_H
#define GENOS_WAKE_UP_TASKLET_H

#include <kernel/sched/Schedee.h>
#include <kernel/event/Waiter.h>
#include <kernel/csection.h>

namespace Genos {

	class WakeUpWaiter : public Waiter {
		Schedee* waitedSchedee;

	public:
		WakeUpWaiter(Schedee* waitedSchedee) : 
			waitedSchedee(waitedSchedee) {}

		void invoke() override {
			//dprln("WAKE UP WAITER INVOKE"); 
			waitedSchedee->unwait();
		}

		Schedee* schedee() {
			return waitedSchedee;
		}
	};

	void wait(WaiterHead& head, WakeUpWaiter& wakeup) {
		//dprln("enter");
		atomic_section_enter();
		//while(1);
		wakeup.schedee()->wait();
		head.wait(wakeup);
		//dprln("leave");
		atomic_section_leave();
	}

	int8_t wait(pid_t pid, WakeUpWaiter& wakeup) {
		auto sch = Genos::raw(pid);
		if (!sch) return -1;
		wait(sch->finalWaiterHead, wakeup);
	}
}

#endif