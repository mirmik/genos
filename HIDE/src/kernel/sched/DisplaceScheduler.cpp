#include <kernel/sched/Scheduler.h>

void Genos::Glue::displace(uint8_t type) {
	RESET_STACK(); 
	schedule();
}