#ifndef GENOS_WAKE_UP_TASKLET_H
#define GENOS_WAKE_UP_TASKLET_H

#include <kernel/sched/Schedee.h>
#include <kernel/event/Waiter.h>
#include <kernel/csection.h>

namespace Genos {

	class WakeUpWaiter : public Waiter, public SchedeeResource {
		Schedee* waitedSchedee;

	public:
		WakeUpWaiter(Schedee* waitedSchedee) : 
			waitedSchedee(waitedSchedee) {

				//dprptrln(this);
			}

		void invoke() override {
			//dprln("invoke");
			atomic_section_enter();
			//dprln("enter");
			waitedSchedee->unwait();
			//dprln("unwait");
			waitedSchedee->unbindResource(this);
			//dprln("leave");
			atomic_section_leave();
			//dprln("after_leave");
		}

		Schedee* schedee() {
			return waitedSchedee;
		}

		void releaseResource() {
			Waiter::unbind();
			SchedeeResource::unbindFromParent();
		}
	};

	void wakeup_wait(WaiterHead& head, WakeUpWaiter& wakeup);/* {
		atomic_section_enter();
		wakeup.schedee()->addResource(wakeup);
		wakeup.schedee()->wait();
		head.wait(wakeup);
		atomic_section_leave();
	}*/

	int8_t wakeup_wait(pid_t pid, WakeUpWaiter& wakeup);/* {
		atomic_section_enter();
		auto sch = Genos::raw(pid);
		if (!sch) return -1;
		wakeup_wait(sch->finalWaiterHead, wakeup);
		atomic_section_leave();
	}*/
}

#endif