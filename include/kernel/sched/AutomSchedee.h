#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/sigslot/delegate.h>
#include <kernel/sched/Scheduler.h>
#include <kernel/sched/Schedee.h>

namespace Genos {
	class AutomSchedee : public Schedee {
	private:
		delegate<void, void*> routine;
		delegate<void, void*> destruct;
		void* local;
	public:
		AutomSchedee(delegate<void, void*> routine, void* local = nullptr,
				delegate<void, void*> destruct = do_nothing) :
			routine(routine), local(local), destruct(destruct) {}
	private:	
		void execute() {
			routine(local);
		}

		void displace() { 
			Genos::Glue::displace(Genos::Glue::AutomateSchedeeType); 
		}

		void finalize() {
			destruct(local);
		}
	};

	class AutomFunctorSchedee : public Schedee {
	public:
		AutomFunctorSchedee(int prio) : Schedee(prio) {}
		AutomFunctorSchedee() {}

		virtual void routine() = 0;

	private:	
		void execute() {
			routine();
		}

		void displace() { 
			Genos::Glue::displace(Genos::Glue::AutomateSchedeeType); 
		}
		
		void finalize() {}
	};
}

#endif