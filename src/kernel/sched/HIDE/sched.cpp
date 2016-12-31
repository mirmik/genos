#include <kernel/sched/sched.h>

#include "gxx/container/dlist.h"
#include "gxx/container/hashtable.h"

#include "kernel/sched/idtable.h"
#include <util/bits.h>
#include <kernel/panic.h>
#include <kernel/sched/query.h>
#include <kernel/sched/autom_schedee.h>
//#include <kernel/id/id.h>
struct schedee* volatile __current_schedee;
uint8_t __nonschedule = 0;

dlist<schedee,&schedee::statelnk> runlist[PRIORITY_TOTAL];
dlist<schedee,&schedee::statelnk> stoplist;

void idlefunc() {}
autom_schedee idle (idlefunc,0);

void scheduler::init() {
	idle.start();
}

int scheduler::schedule() {
	if (__nonschedule) return ERRSCHEDBLOCK;

	int priolvl;
	for (priolvl = PRIORITY_TOTAL - 1; priolvl >= 0; --priolvl)
	{
		if (!runlist[priolvl].is_empty()) goto runschedee;
	}
	panic("nobody to run");

	runschedee:
	schedee& sch = *runlist[priolvl].begin();
	scheduler::set_current(&sch);

	while (sch.signals & sch.signals_mask) sch.signal_handler();
	if (sch.is_ready()) sch.execute();
	if (sch.is_ready()) runlist[sch.dyn_prio].push_back(sch);
}

void scheduler::set_current(schedee* sch) {
	__current_schedee = sch;
}

schedee* scheduler::get_current() {
	return __current_schedee;
}

void scheduler::set_ready(schedee* sch, int8_t opt) {
	bits_mask_assign(sch->flags,SCHEDEE_STATE_MASK,SCHEDEE_READY);
	int8_t prio = sch->dyn_prio;
	if (opt)
		runlist[sch->dyn_prio].push_front(*sch);
	else
		runlist[sch->dyn_prio].push_back(*sch);		
};

void scheduler::set_held(schedee* sch) {
	bits_mask_assign(sch->flags,SCHEDEE_STATE_MASK,SCHEDEE_HELD);
	stoplist.push_front(*sch);
};

void scheduler::set_block(schedee* sch, uint16_t state) {
	bits_mask_assign(sch->flags,SCHEDEE_STATE_MASK,state);
	stoplist.push_front(*sch);
};