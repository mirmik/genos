#include <sched/api.h>
#include <sched/timer.h>

void schedee_exit() {
	struct schedee * sch;

	sch = current_schedee();
	schedee_final(sch);
	
	__displace__();
}

void __unsleep(void * priv, struct ktimer * tim) {
	struct schedee * sch = (struct schedee *) priv;
	schedee_run(sch);
	ktimer_dealloc(tim);
}

int schedee_sleep_for_milliseconds(int ms) {
	struct schedee * sch;

	sch = current_schedee();

	if (!sch->flag.can_displace) return -1;

	ktimer_create_for_milliseconds(__unsleep, sch, ms);
	schedee_wait(sch);

	return __displace__();
}