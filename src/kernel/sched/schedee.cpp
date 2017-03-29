#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>

namespace genos {

	/*void schedee_init(struct schedee* sch, 
		const struct schedee_operations* schops, 
		const struct service_operations* srvsops) 
	{
		dlist_init(&sch->lnk);
		service_init(&sch->srvs);
		sch->prio = 0;
		sch->flags = 0;
	
		sch->srvs.hops = srvsops;
		sch->schops = schops;
	}*/
	
	void schedee::run() {
		//debug_printhex_ptr(sch);dln();
		set_state_run();
	}
	
	void schedee::stop() {
		set_state_wait(SCHEDEE_BLOCKED_STOP);
	}
	
	//void schedee::exit() {
	//	struct schedee* cursch = current_schedee();
	//	set_state_final(cursch);
	//	cursch->schops->lastexit(cursch);
	//}
	
	void finalize_schedee(struct schedee* sch) {
		sch->set_state_zombie();
	
		//if (final_callback) final_callback(sch);
	
		if (sch->is_final_deallocated()) { 
			sch->destructor();
			scheduler_free(sch);
		}
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