#ifndef GENOS_DELEGATE_WAITER_H
#define GENOS_DELEGATE_WAITER_H

#include <kernel/event/Waiter.h>
#include <genos/sigslot/delegate.h>

namespace Genos {
	class DelegateWaiter : public Waiter {
		delegate<void> dlg;
	public:
		DelegateWaiter(delegate<void> dlg) : dlg(dlg) {}

		void invoke() override {
			dlg();
		}
	};
}

#endif