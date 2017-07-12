#include <kernel/sched/sched.h>

#include "gxx/DList.h"

#include <gxx/util/bits.h>

#include <genos/panic.h>
#include <kernel/sched/automschedee.h>

Kernel::schedee* volatile __current_schedee;
uint8_t __nonschedule = 0;

namespace Kernel {

	gxx::DList<schedee,&schedee::statelnk> runlist[PRIORITY_TOTAL];
	gxx::DList<schedee,&schedee::statelnk> stoplist;

	void idle() {

	}

	automSchedee idle_schedee (idle,0);

	void sched_init() {
		idle_schedee.start();
	}

	void set_ready(schedee* sch, int8_t opt) {
		bits_mask_assign(sch->flags,SCHEDEE_STATE_MASK,SCHEDEE_READY);
		int8_t prio = sch->dyn_prio;
		if (opt)
			runlist[sch->dyn_prio].push_front(*sch);
		else
			runlist[sch->dyn_prio].push_back(*sch);		
	};

	void set_held(schedee* sch) {
		bits_mask_assign(sch->flags,SCHEDEE_STATE_MASK,SCHEDEE_HELD);
		stoplist.push_front(*sch);
	};

	void set_block(schedee* sch, uint16_t state) {
		bits_mask_assign(sch->flags,SCHEDEE_STATE_MASK,state);
		stoplist.push_front(*sch);
	};

	int schedule() {
		if (__nonschedule) return ERRSCHEDBLOCK;

		int priolvl;
		for (priolvl = PRIORITY_TOTAL - 1; priolvl >= 0; --priolvl)
		{
			if (!runlist[priolvl].is_empty()) goto runschedee;
		}
		panic("nobody to run");

		runschedee:
		schedee& sch = *runlist[priolvl].begin();
		
		Kernel::currentSchedee(&sch);

		//while (sch.signals & sch.signals_mask) sch.signal_handler();
		if (sch.is_ready()) sch.execute();
		if (sch.is_ready()) runlist[sch.dyn_prio].push_back(sch);
	}

	void currentSchedee(schedee* sch) {
		__current_schedee = sch;
	}

	schedee* currentSchedee() {
		return (schedee*)__current_schedee;
	}

}