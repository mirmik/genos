#include <loc_scheduler.h>

#include <asm/startup.h>

void scheduler_free(void*) __attribute__((weak));
void scheduler_free(void* ptr) { free(ptr); };

void __schedee_plan__() {
	if (__planned_schedee) return;

	while (!dlist_empty(&finallist)) {
		struct schedee* fsch = dlist_first_entry(&finallist, struct schedee, lnk);
		finalize_schedee(fsch);
	}

	int priolvl;
	for (priolvl = PRIORITY_TOTAL - 1; priolvl >= 0; --priolvl) {
		if (!dlist_empty(&runlist[priolvl])) goto set;
	}
	return;

	set:
	__planned_schedee = dlist_first_entry(&runlist[priolvl], struct schedee, lnk);
	dlist_move_back(&__planned_schedee->lnk, &runlist[priolvl]);
}

int iterr = 0;

void __schedule__() {
	__label__ __schedule_loop;
	__schedule_loop:

	//debug_print("__displace_schedule__");

	RESET_STACK();

	if (__planned_schedee) {
		__current_schedee = __planned_schedee;
		__planned_schedee = NULL;
	//	debug_printdec_int32(iterr++);
		__current_schedee->schops->engage(__current_schedee);
	}
	else __schedee_plan__();

	goto __schedule_loop;
}

void __displace__() {
	__current_schedee->schops->displace(__current_schedee);
}