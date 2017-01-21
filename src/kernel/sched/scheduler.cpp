#include <kernel/sched/scheduler.h>

#include <gxx/DList.h>
#include <util/bits.h>

gxx::DList<schedee, &schedee::lnk> runlist;
gxx::DList<schedee, &schedee::lnk> waitlist;
gxx::DList<schedee, &schedee::lnk> zombielist;

bool __noschedule = false;

void set_state_run(schedee *sch) {
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_RUN);
}

void set_state_wait(schedee *sch) {
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_WAIT);
}

void set_state_zombie(schedee *sch) {
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_ZOMBIE);
}

void scheduler_block() {
	__nonschedule = true;
}


void scheduler_unblock() {
	scheduler_unblock = false;
}

int schedule() {
	if (__nonschedule) return 1;

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
