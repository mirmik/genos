#include <kernel/sched/schedee.h>
#include <kernel/sched/sched.h>

void Kernel::schedee::start() {
	Kernel::set_ready(this, 0);
}

void Kernel::schedee::stop() {
	//scheduler::set_held(*this);
	//signal::send(this, SIGSTOP);
}

