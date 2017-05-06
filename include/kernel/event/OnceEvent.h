#ifndef GENOS_DELEGATE_WAITER_H
#define GENOS_DELEGATE_WAITER_H

#include <kernel/event/Waiter.h>

namespace Genos {
	class OnceEvent : public WaiterHead {
		WaiterList list;

	public:
		void wait(Waiter& waiter) override {
			list.move_back(waiter);
		}

		void invoke() {
			//dprln("OnceEvent::invoke");
			while(!list.empty()) {
				Waiter& waiter = *list.begin();
				waiter.unbind_and_invoke();
			}
		}
	};
}

#endif

