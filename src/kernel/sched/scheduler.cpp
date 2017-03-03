#include <kernel/sched/scheduler.h>

#include <gxx/DList.h>
#include <util/bits.h>
#include <utilxx/setget.h>
#include <kernel/tasks/tasklet.h>

gxx::DList<schedee, &schedee::lnk> runlist[PRIORITY_TOTAL];
gxx::DList<schedee, &schedee::lnk> waitlist;
gxx::DList<schedee, &schedee::lnk> finallist;

stub_schedee __stubschedee;
schedee* __current_schedee = &__stubschedee;
schedee* current_schedee() { return __current_schedee; }
void current_schedee(schedee* sch) { __current_schedee = sch; }

static bool __noschedule = false;

void set_state_run(schedee *sch) {
	assert(!bits_mask(sch->flags, SCHEDEE_STATE_ZOMBIE));
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_RUN);
	runlist[sch->prio].move_back(*sch);
}

void set_state_wait(schedee *sch) {
	if (bits_mask(sch->flags, SCHEDEE_STATE_ZOMBIE)) return;
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_WAIT);
	waitlist.move_back(*sch);
}

void set_state_final(schedee *sch) {
	assert(!bits_mask(sch->flags, SCHEDEE_STATE_ZOMBIE));
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_FINAL);
	finallist.move_back(*sch);
}

void set_state_zombie(schedee *sch) {
	debug_print("set_state_zombie "); dprhexln((uintptr_t)sch);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_ZOMBIE);
	dlist_del_init(&sch->lnk);
//	zombielist.move_back(*sch);
}

void set_final_deallocate(schedee *sch, bool en) {
	bits_lvl_mask(sch->flags, SCHEDEE_FINAL_DEALLOCATE, en);
}

void set_final_release(schedee *sch, bool en) {
	bits_lvl_mask(sch->flags, SCHEDEE_FINAL_RELEASE, en);
}

bool state_is_run(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_RUN);
}

bool state_is_wait(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_WAIT);
}

bool state_is_zombie(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_ZOMBIE);
}

bool is_final_deallocated(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_FINAL_DEALLOCATE);
}

bool is_final_release(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_FINAL_RELEASE);
}

void scheduler_block() {
	__noschedule = true;
}

void scheduler_unblock() {
	__noschedule = false;
}

void genos_idle()__attribute__((weak));
void genos_idle() {}

void scheduler_free(void*) __attribute__((weak));
void scheduler_free(void* ptr) { free(ptr); };

void __schedule__() {
	__label__ __schedule_loop;

	__schedule_loop:

	tasklet_schedule();

	while (!finallist.empty()) {
		schedee& fsch = *finallist.begin();

		//Операция final должна озаботиться извлечением процесса из списка final.
		//Иначе возможен вечный вызов данного финализатора.
		fsch.final();
	}

	int priolvl;
	for (priolvl = PRIORITY_TOTAL - 1; priolvl >= 0; --priolvl) {
		if (!runlist[priolvl].empty()) goto runschedee;
	}
	genos_idle();
	goto __schedule_loop;	
	//panic("nobody to run");

	runschedee:
	schedee& sch = *runlist[priolvl].begin();
	
	current_schedee(&sch);

	//Перемещаем в конец списка.
	runlist[priolvl].move_back(sch);
	sch.execute();

	//while (sch.signals & sch.signals_mask) sch.signal_handler();
	//if (sch.is_ready()) sch.execute();
	//if (sch.is_ready()) runlist[sch.dyn_prio].move_back(sch);

	goto __schedule_loop;
}
