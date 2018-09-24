#include <sched/sched.h>

#define SCHEDEE_PRIORITY_MAXIMUM 4

static struct dlist_head run_list[SCHEDEE_PRIOMAX];
static struct dlist_head wait_list;
//struct dlist_head zombie_list;

struct schedee * __current_schedee = NULL;
struct schedee * current_schedee() {
	return __current_schedee;
}


void schedee_run(struct schedee * sch) {
	dlist_move(&sch->lnk, &run_list[sch->prio]);
}

void schedee_wait(struct schedee * sch) {
	dlist_move(&sch->lnk, &wait_list);
}

void schedee_stop(struct schedee * sch);
void schedee_finish(struct schedee * sch);

void schedee_manager() {
	/*while (!finallist.empty()) {
		schedee& fsch = *finallist.begin();
				
		//данная функция инициализирует процедуру удаления
		//процесса из системы. Удаление может быть достаточно
		//длительным, но в любом случае это не проблемы планировщика.
		schedee_list::unbind(fsch);
		fsch.finalize();
	}*/
		
	//atomic_section_enter();
	system_lock();

	schedee* sch;
	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++) {
		if (!runlist[priolvl].empty()) {
					
			sch = &*runlist[priolvl].begin();
			runlist[priolvl].move_back(*sch);
			//__current_schedee = sch;
			system_unlock();
			schedee_execute(sch);
			return;
		}
	}

	system_unlock();

	//Nobody to run
	return;
}

int __displace__();

int displace_automic() {
	struct schedee * cur = __current_schedee;

}