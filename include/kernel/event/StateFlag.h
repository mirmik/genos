#ifndef GENOS_STATE_FLAG_H
#define GENOS_STATE_FLAG_H

#include <kernel/event/Waiter.h>
#include <genos/sigslot/delegate.h>
#include <kernel/csection.h>

namespace Genos {
	class StateFlag : public WaiterHead {
		bool flag = false;
		WaiterList list;

	public:
		bool is_set() {
			return flag;
		}

		void wait(Waiter& waiter) override {
			atomic_section_enter();
			if (flag) waiter.invoke();
			else list.move_back(waiter);
			atomic_section_leave();
		}

		void set() {
			atomic_section_enter();
			flag = true;

			while(!list.empty()) {
				Waiter& waiter = *list.begin();
				waiter.unbind_and_invoke();
			}
			atomic_section_leave();
		}

		void reset() {
			flag = false;
		}

		//int status() {
		//	return list.size();
		//}
	};
}

#endif
