#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/sigslot/delegate.h>
#include <kernel/sched/Scheduler.h>
#include <kernel/sched/Schedee.h>

#include <kernel/stdstream.h>

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

		void destroy() override { delete this; }
	};

	class AutomShellSchedee : public Schedee {
	private:
		delegate<void, int, char**> routine;
		str_argvc_t argvc;

	public:
		AutomShellSchedee(delegate<void, int, char**> routine, str_argvc_t& argvc) :
			routine(routine), argvc(argvc) {}
	private:	
		void execute() {
			routine(argvc.argc(), argvc.argv());
		}

		void destruct() {
			free(argvc.str);
		}

		void displace() { 
			Genos::Glue::displace(Genos::Glue::AutomateSchedeeType); 
		}

		void finalize() {
			destruct();
			finalizeSchedee();
		}

		void destroy() override { delete this; }
	};

	class AutomFunctorSchedee : public Schedee {
	public:
		StdStreamStruct strm;

		AutomFunctorSchedee(int prio) : 
			Schedee(prio) {}

		AutomFunctorSchedee() {}

		virtual void routine() = 0;
		virtual void destruct() {}

	private:	
		void execute() {
			currentStdStream(&strm);
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
		auto sch = new AutomFunctor(gxx::forward<Args>(args) ...);

		Schedee* cur = Genos::currentSchedee();
		sch->initFromParent(cur, CLONE_DESCRIPTORS | LINK_AS_RESOURCE);

		sch->strm = *currentStdStream();
		
		auto pid = Genos::Glue::registerSchedee(sch);
		return pid;
	}

	//template<typename AutomFunctor, typename ... Args>
	pid_t automSchedeeCreate(
		delegate<void, void*> 	routine, 
		void* 					local, 
		delegate<void, void*> 	destruct = do_nothing) 
	{
		auto sch = new AutomSchedee(routine, local,	destruct);

		Schedee* cur = Genos::currentSchedee();
		sch->initFromParent(cur, CLONE_DESCRIPTORS | LINK_AS_RESOURCE);

		//sch->strm = *currentStdStream();
		
		auto pid = Genos::Glue::registerSchedee(sch);
		return pid;
	}

	pid_t automShellSchedeeCreate(delegate<void, int, char**> routine, str_argvc_t& argvc) 
	{
		auto sch = new AutomShellSchedee(routine, argvc);

		Schedee* cur = Genos::currentSchedee();
		sch->initFromParent(cur, CLONE_DESCRIPTORS | LINK_AS_RESOURCE);

		//sch->strm = *currentStdStream();
		
		auto pid = Genos::Glue::registerSchedee(sch);
		return pid;
	}
}

#endif