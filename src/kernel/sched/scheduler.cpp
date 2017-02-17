#include <kernel/sched/scheduler.h>

#include <gxx/DList.h>
#include <util/bits.h>
#include <utilxx/setget.h>
#include <kernel/tasks/tasklet.h>

gxx::DList<schedee, &schedee::lnk> runlist[PRIORITY_TOTAL];
gxx::DList<schedee, &schedee::lnk> waitlist;
gxx::DList<schedee, &schedee::lnk> zombielist;

schedee* __current_schedee;
auto current_schedee() { return __current_schedee; }
void current_schedee(schedee* sch) { __current_schedee = sch; }

static bool __noschedule = false;

void set_state_run(schedee *sch) {
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_RUN);
	runlist[sch->prio].push_back(*sch);
}

void set_state_wait(schedee *sch) {
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_WAIT);
	waitlist.push_back(*sch);
}

void set_state_zombie(schedee *sch) {
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_ZOMBIE);
	zombielist.push_back(*sch);
}

void scheduler_block() {
	__noschedule = true;
}

void scheduler_unblock() {
	__noschedule = false;
}

void __schedule__() {
	__label__ __schedule_loop;

	__schedule_loop:

	tasklet_schedule();

	int priolvl;
	for (priolvl = PRIORITY_TOTAL - 1; priolvl >= 0; --priolvl) {
		if (!runlist[priolvl].empty()) goto runschedee;
	}
	goto __schedule_loop;	
	//panic("nobody to run");

	runschedee:
	schedee& sch = *runlist[priolvl].begin();
	
	current_schedee(&sch);

	//Перемещаем в конец списка.
	runlist[priolvl].push_back(sch);
	sch.execute();

	//while (sch.signals & sch.signals_mask) sch.signal_handler();
	//if (sch.is_ready()) sch.execute();
	//if (sch.is_ready()) runlist[sch.dyn_prio].push_back(sch);

	goto __schedule_loop;
}
