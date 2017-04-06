#ifndef GENOS_WAITER_H
#define GENOS_WAITER_H

#include <gxx/dlist.h>
#include <genos/sigslot/delegate.h>

namespace Genos {

	class Waiter {
	public:
		dlist_head lnk;

		Waiter() {
			dlist_init(&lnk);
		}

		virtual void invoke() = 0;

		inline void unbind() {
			dlist_del_init(&lnk);
		}

		void unbind_and_invoke() {
			dlist_del_init(&lnk);
			invoke();	
		}
	};

	class WaiterHead {
	public:
		virtual void wait(Waiter& waiter) = 0;
	};

	class DelegateWaiter : public Waiter {
		delegate<void> dlg;
	public:
		DelegateWaiter(delegate<void> dlg) : dlg(dlg) {}

		void invoke() override {
			dlg();
		}
	};

	using WaiterList = gxx::dlist<Waiter, &Waiter::lnk>;
}

#endif