#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>

void schedee::Run() {
	set_state_run(this);
}

void schedee::Stop() {
	set_state_wait(this);
}

void schedee::Exit() {
	set_state_zombie(this);
}