#ifndef GENOS_EVENT_H
#define GENOS_EVENT_H

#include <kernel/event/Waiter.h>
#include <genos/sigslot/delegate.h>

namespace Genos {
	
	class Tasklet;
	namespace Glue {
		void taskletToExecute(Tasklet&);
	}

	class Tasklet : public Waiter {
	public:
		bool fast = false;;
		
		void invoke() override {
			if (fast) routine();
			else Genos::Glue::taskletToExecute(*this);
		}

		virtual void routine() = 0;
	};

	class StateFlag : public WaiterHead {
		bool flag = false;
		WaiterList list;

	public:
		void wait(Waiter& waiter) override {
			if (flag) waiter.invoke();
			else list.move_back(waiter);
		}

		void set() {
			flag = true;
			while(!list.empty()) {
				Waiter& waiter = *list.begin();
				waiter.unbind_and_invoke();
			}
		}

		void reset() {
			flag = false;
		}
	};

	class DelegateTasklet : public Tasklet {
	public:
		DelegateTasklet(delegate<void> dlg) : m_delegate(dlg) {}

		void routine() {
			m_delegate();
		} 

	private:
		delegate<void> m_delegate;
	};

	class DebugTasklet : public Tasklet {
		void routine() {
			dprln("DebugTasklet");
		}
	};
}

#endif