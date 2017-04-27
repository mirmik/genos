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
			finalizeSchedee();
		}
	};

	class AutomFunctorSchedee : public Schedee {
	public:
		AutomFunctorSchedee(int prio) : 
			Schedee(prio) {}

		AutomFunctorSchedee() {}

		virtual void routine() = 0;
		virtual void destruct() {}

	private:	
		void execute() {
			routine();
		}

		void displace() { 
			Genos::Glue::displace(Genos::Glue::AutomateSchedeeType); 
		}
		
		void finalize() {
			destruct();
			finalizeSchedee();
		}
	};

	template<typename AutomFunctor, typename ... Args>
	pid_t automFunctorCreate(Args ... args) {
		Schedee* sch = new AutomFunctor(gxx::forward<Args>(args) ...);
		Schedee* cur = Genos::currentSchedee();
		sch->setParent(cur);
		
		auto pid = Genos::Glue::registerSchedee(sch);
		return pid;
	}
}

#endif