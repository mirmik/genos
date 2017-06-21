#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>

void schedee_init(struct schedee* sch, 
	const struct schedee_operations* schops, 
	const struct service_operations* srvsops) 
{
	dlist_init(&sch->lnk);
	service_init(&sch->srvs);
	sch->prio = 0;
	sch->flags = 0;

	sch->srvs.hops = srvsops;
	sch->schops = schops;
}

void schedee_run(struct schedee* sch) {
	//debug_printhex_ptr(sch);dln();
	schedee_set_state_run(sch);
}

void schedee_stop(struct schedee* sch) {
	schedee_set_state_wait(sch, SCHEDEE_BLOCKED_STOP);
}

void schedee_exit() {
	struct schedee* cursch = current_schedee();
	schedee_set_state_final(cursch);
	cursch->schops->lastexit(cursch);
}

void finalize_schedee(struct schedee* sch) {
	schedee_set_state_zombie(sch);

	//if (final_callback) final_callback(sch);

	if (is_final_deallocated(sch)) { 
		sch->schops->destructor(sch);
		scheduler_free(sch);
	}
}

//schedee& schedee_final_deallocate(bool en) {
//	set_final_deallocate(this, en);
//	return *this;
//}

//schedee& schedee::final_handler(void(*f)(schedee*)) {
	//if (final_callback == nullptr);
//	final_callback = f;
//}