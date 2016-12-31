#include "kernel/sched/schedee.h"
#include "kernel/signals.h"
#include "kernel/sched/procmngr.h"

void signal::send(schedee* sch, sig_t sig) {
	bits_set(sch->signals, sig);
}

void signal::send(id_t pid, sig_t sig) {
	send(process::getsch(pid),sig);
}

