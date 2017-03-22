#include <kernel/sched/scheduler.h>
#include <kernel/sched/stubschedee.h>

#include <loc_scheduler.h>

#include <util/bits.h>



genos::schedee* __planned_schedee = NULL;
genos::schedee* planned_schedee() { return __planned_schedee; }
void set_planned_schedee(genos::schedee* sch) { __planned_schedee = sch; }

genos::schedee* __current_schedee = NULL;
genos::schedee* current_schedee() { return __current_schedee; }
void set_current_schedee(genos::schedee* sch) { __current_schedee = sch; }

gxx::dlist<genos::schedee, &genos::schedee::schlnk> runlist[PRIORITY_TOTAL];
gxx::dlist<genos::schedee, &genos::schedee::schlnk> waitlist;
gxx::dlist<genos::schedee, &genos::schedee::schlnk> finallist;

//struct dlist_head runlist[PRIORITY_TOTAL];
//struct dlist_head waitlist;
//struct dlist_head finallist;

void scheduler_init() {
	/*for (int i = 0; i < PRIORITY_TOTAL; i++)
		dlist_init(&runlist[i]);
	dlist_init(&waitlist);
	dlist_init(&finallist);*/
}

static bool __noschedule = false;

//void kernel_stub_schedee_init() {
	//__current_schedee = (genos::schedee*) construct_stub_schedee();
//}

bool genos::schedee::state_is(uint8_t state) {
	//debug_printdec_int32(flags);dln();
	//debug_printdec_int32(state);dln();
	return bits_mask(flags, SCHEDEE_STATE_MASK) == state;
}

void genos::schedee::set_state_run() {
	assert((flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_RUN);
	runlist[prio].move_back(*this);
	//debug_print("HERE\r\n");
	//debug_printhex_ptr(sch);dln();
	//debug_printhex_ptr(&schlnk);dln();
	//debug_printhex_ptr(&runlist[prio]);dln();
}

void genos::schedee::set_state_wait(uint8_t state) {
	assert((flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE && (flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_RUN);
	bits_mask_assign(flags, SCHEDEE_STATE_MASK, state);
	waitlist.move_back(*this);
}

void genos::schedee::set_state_final() {
	assert((flags & SCHEDEE_STATE_MASK) != SCHEDEE_STATE_ZOMBIE);
	bits_mask_assign(flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_FINAL);
	finallist.move_back(*this);
}

void genos::schedee::set_state_zombie() {
	//debug_print("set_state_zombie "); dprhexln((uintptr_t)sch);
	bits_mask_assign(flags, SCHEDEE_STATE_MASK, SCHEDEE_STATE_ZOMBIE);
	dlist_del_init(&schlnk);
//	zombielist.move_back(*sch);
}

void genos::schedee::set_final_deallocate(bool en) {
	bits_lvl_mask(flags, SCHEDEE_FINAL_DEALLOCATE, en);
}

void genos::schedee::set_final_release(bool en) {
	bits_lvl_mask(flags, SCHEDEE_FINAL_RELEASE, en);
}
/*
bool state_is_run(genos::schedee *sch) {
	return bits_mask(flags, SCHEDEE_STATE_RUN);
}

bool state_is_wait(genos::schedee *sch) {
	return bits_mask(flags, SCHEDEE_STATE_WAIT);
}

bool state_is_zombie(genos::schedee *sch) {
	return bits_mask(flags, SCHEDEE_STATE_ZOMBIE);
}*/

bool genos::schedee::is_final_deallocated() {
	return bits_mask(flags, SCHEDEE_FINAL_DEALLOCATE);
}

bool genos::schedee::is_final_release() {
	return bits_mask(flags, SCHEDEE_FINAL_RELEASE);
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
