#include <kernel/sched/scheduler.h>
#include <kernel/sched/stubschedee.h>

#include <gxx/DList.h>
#include <util/bits.h>
#include <utilxx/setget.h>
#include <kernel/tasks/tasklet.h>

stub_schedee __stubschedee;

schedee* __current_schedee = &__stubschedee;
schedee* current_schedee() { return __current_schedee; }
void current_schedee(schedee* sch) { __current_schedee = sch; }

gxx::DList<schedee, &schedee::lnk> runlist[PRIORITY_TOTAL];
gxx::DList<schedee, &schedee::lnk> waitlist;
gxx::DList<schedee, &schedee::lnk> finallist;

static bool __noschedule = false;

bool schedee_state_is(schedee *sch, uint8_t state) {
	return bits_mask(sch->flags, SCHEDEE_STATE_MASK) == state;
}

void schedee_set_state_run(schedee *sch) {
	assert((sch->flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_RUN);
	runlist[sch->prio].move_back(*sch);
}

void schedee_set_state_wait(schedee *sch, uint8_t state) {
	assert((sch->flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, state);
	if (state == SCHEDEE_STATE_RUN) {
		runlist[sch->prio].move_back(*sch);
	}
	else {
		waitlist.move_back(*sch);
	} 
}

void schedee_set_state_final(schedee *sch) {
	assert((sch->flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_FINAL);
	finallist.move_back(*sch);
}

void schedee_set_state_zombie(schedee *sch) {
	//debug_print("set_state_zombie "); dprhexln((uintptr_t)sch);
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
/*
bool state_is_run(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_RUN);
}

bool state_is_wait(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_WAIT);
}

bool state_is_zombie(schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_ZOMBIE);
}*/

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

void kernel_schedule_empty()__attribute__((weak));
void kernel_schedule_empty() {
	panic("kernel_schedule_empty");
}

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
	dprln(waitlist.size());
	if (waitlist.empty()) kernel_schedule_empty();
	genos_idle();
	goto __schedule_loop;	
	//panic("nobody to run");

	runschedee:
	schedee& sch = *runlist[priolvl].begin();
	
	//if (current_schedee() != &sch) current_schedee()->displace(); 	
	current_schedee(&sch);

	//Перемещаем в конец списка.
	runlist[priolvl].move_back(sch);
	
	//Если execute вернет SCHEDULE_RESUME (1), цикл завершается. 
	//SCHEDULE_REPEAT (0) ведет к следующей итерации.
	if (sch.execute()) return;
	goto __schedule_loop;
}

void __displace__() {
	__schedule__();
}