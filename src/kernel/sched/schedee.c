#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>

void schedee_init(struct schedee* sch) {
	dlist_init(&sch->lnk);
	service_init(&sch->srvs);
	sch->prio = 0;
	sch->flags = 0;
}

void schedee_run(struct schedee* sch) {
	schedee_set_state_run(sch);
}

void schedee_stop(struct schedee* sch) {
	schedee_set_state_wait(sch, SCHEDEE_BLOCKED_STOP);
}

//bool schedee::Completed(struct schedee* sch) {
//	return state_is_zombie(this);
//}

void schedee_exit(struct schedee* sch) {
	schedee_set_state_final(sch);
	sch->schops->lastexit(sch);
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