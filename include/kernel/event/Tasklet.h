#ifndef GENOS_EVENT_H
#define GENOS_EVENT_H

#include <gxx/dlist.h>
#include <genos/sigslot/delegate.h>

namespace Genos {
	
	class Tasklet;
	namespace Glue {
		void taskletToExecute(Tasklet&);
	}

	class Tasklet {
	public:
		dlist_head lnk;
		bool fast = false;;
		
		Tasklet() {
			dlist_init(&lnk);
		}

		void invoke() {
			//dprln("Tasklet::invoke");
			if (fast) routine();
			else Genos::Glue::taskletToExecute(*this);
		}

		virtual void routine() = 0;
	};

	using TaskletList = gxx::dlist<Tasklet, &Tasklet::lnk>;

	class DebugTasklet : public Tasklet {
		void routine() {
			dprln("DebugTasklet");
		}
	};

	class StateFlag {
		bool flag = false;
		TaskletList list;

	public:
		void wait(Tasklet& tasklet) {
			if (flag) tasklet.invoke();
			else list.move_back(tasklet);
		}

		void set() {
			flag = true;
			while(!list.empty()) {
				Tasklet& tasklet = *list.begin();
				list.del_init(tasklet);
				tasklet.invoke();
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

}

#endif