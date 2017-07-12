#include <kernel/sched/scheduler.h>
#include <kernel/sched/schedee.h>
#include <kernel/sched/stubschedee.h>

#include <loc_scheduler.h>

#include <gxx/util/bits.h>

struct schedee* __planned_schedee = NULL;
struct schedee* planned_schedee() { return __planned_schedee; }
void set_planned_schedee(struct schedee* sch) { __planned_schedee = sch; }

struct schedee* __current_schedee = NULL;
struct schedee* current_schedee() { return __current_schedee; }
void set_current_schedee(struct schedee* sch) { __current_schedee = sch; }

//gxx::DList<schedee, &schedee::lnk> runlist[PRIORITY_TOTAL];
//gxx::DList<schedee, &schedee::lnk> waitlist;
//gxx::DList<schedee, &schedee::lnk> finallist;

struct dlist_head runlist[PRIORITY_TOTAL];
struct dlist_head waitlist;
struct dlist_head finallist;

void scheduler_init() {
	for (int i = 0; i < PRIORITY_TOTAL; i++)
		dlist_init(&runlist[i]);
	dlist_init(&waitlist);
	dlist_init(&finallist);
}

static bool __noschedule = false;

void kernel_stub_schedee_init() {
	__current_schedee = (struct schedee*) construct_stub_schedee();
}

bool schedee_state_is(struct schedee *sch, uint8_t state) {
	//debug_printdec_int32(sch->flags);dln();
	//debug_printdec_int32(state);dln();
	return bits_mask(sch->flags, SCHEDEE_STATE_MASK) == state;
}

void schedee_set_state_run(struct schedee *sch) {
	assert((sch->flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_RUN);
	dlist_move_back(&sch->lnk, &runlist[sch->prio]);
	//debug_print("HERE\r\n");
	//debug_printhex_ptr(sch);dln();
	//debug_printhex_ptr(&sch->lnk);dln();
	//debug_printhex_ptr(&runlist[sch->prio]);dln();
}

void schedee_set_state_wait(struct schedee *sch, uint8_t state) {
	assert((sch->flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, state);
	if (state == SCHEDEE_STATE_RUN) {
		dlist_move_back(&sch->lnk, &runlist[sch->prio]);
	}
	else {
		dlist_move_back(&sch->lnk, &waitlist);
	} 
}

void schedee_set_state_final(struct schedee *sch) {
	assert((sch->flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_FINAL);
	dlist_move_back(&sch->lnk, &finallist);
}

void schedee_set_state_zombie(struct schedee *sch) {
	//debug_print("set_state_zombie "); dprhexln((uintptr_t)sch);
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_ZOMBIE);
	dlist_del_init(&sch->lnk);
//	zombielist.move_back(*sch);
}

void set_final_deallocate(struct schedee *sch, bool en) {
	bits_lvl_mask(sch->flags, SCHEDEE_FINAL_DEALLOCATE, en);
}

void set_final_release(struct schedee *sch, bool en) {
	bits_lvl_mask(sch->flags, SCHEDEE_FINAL_RELEASE, en);
}
/*
bool state_is_run(struct schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_RUN);
}

bool state_is_wait(struct schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_WAIT);
}

bool state_is_zombie(struct schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_STATE_ZOMBIE);
}*/

bool is_final_deallocated(struct schedee *sch) {
	return bits_mask(sch->flags, SCHEDEE_FINAL_DEALLOCATE);
}

bool is_final_release(struct schedee *sch) {
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
