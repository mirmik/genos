#include <genos/sched/scheduler.h>

schedee_manager_t schmngr;

void schedee_manager_init(/*schedee_manager_t* schmngr*/) {
	for (int i = 0; i < PRIORITY_TOTAL; i++) {
		dlist_init(&schmngr.run_list[i]);
	}
	dlist_init(&schmngr.wait_list);
	dlist_init(&schmngr.final_list);
	dlist_init(&schmngr.zombie_list);
}

void schedee_manager_set_state(/*schedee_manager_t* schmngr*/ schedee_t* sch, uint8_t state) {
	assert(sch->prio < PRIORITY_TOTAL);

	sch->state = state;

	struct dlist_head* target_list;
	switch (state) {
		case SCHEDEE_STATE_RUN: target_list = &schmngr.run_list[sch->prio]; break;
		case SCHEDEE_STATE_WAIT: target_list = &schmngr.wait_list; break;
		case SCHEDEE_STATE_ZOMBIE: target_list = &schmngr.zombie_list; break;
		case SCHEDEE_STATE_FINAL: target_list = &schmngr.final_list; break;
		default: panic("unrecognized schedee state");
	}
	dlist_move_back(&sch->schlnk, target_list);
}

void schedee_manager_schedule(/*schedee_manager_t* schmngr*/) {
	while ( !dlist_empty(&schmngr.final_list) ) {
		schedee_t* fsch = dlist_first_entry(&schmngr.final_list, schedee_t, schlnk);
		
		//данная функция инициализирует процедуру удаления
		//процесса из системы. Удаление может быть достаточно
		//длительным, но в любом случае это не проблемы планировщика.
		dlist_del(&fsch->schlnk);
		schedee_finalize(fsch);
	}
	
	//atomic_section_enter();

	schedee_t* sch;
	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++) {
		if (! dlist_empty(&schmngr.run_list[priolvl])) {
			
			sch = dlist_first_entry(&schmngr.run_list[priolvl], schedee_t, schlnk);
			dlist_move_back(&sch->schlnk, &schmngr.run_list[priolvl]);
			schmngr.cursch = sch;

			//critical_section_leave();
			schedee_execute(sch);
			return;

		}
	}

	//atomic_section_leave();

	//Nobody to run
	return;
}

//pid_t current_schedee_pid() {
//	return schmngr->cursch->pid;
//}

schedee_t* current_schedee() {
	return schmngr.cursch;
}

void __displace__() {
	schmngr.cursch->ops->displace(schmngr.cursch);
}