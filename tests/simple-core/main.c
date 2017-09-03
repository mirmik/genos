#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpiotbl.h>

#include <genos/systime.h>
#include <gxx/debug/dprint.h>

#include <genos/sched/scheduler.h>
#include <genos/sched/autom_schedee.h>
#include <genos/sched/sleep.h>
#include <genos/time/stimer.h>

void schedule();

void setup() {
	board_init();
	schedee_manager_init();
	stimer_manager_init();

	global_irqs_enable();
}

void func(void* data) {
	debug_print("HelloWorld");
	sleep_for_milliseconds(1000);
}

int main() {
	setup();

	//while(1) {
	//	pinnum_set_level(13, 1);
	//	delay(1000);
	//	pinnum_set_level(13, 0);
	//	delay(1000);
	//}	

	autom_schedee_t asch;
	autom_schedee_init(&asch, func, NULL);
	schedee_manager_set_state(&asch.sch, SCHEDEE_STATE_RUN);

	while(1) {
		schedee_manager_schedule();
	}
}