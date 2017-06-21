#include <kernel/sched/Scheduler.h>

void Genos::Glue::displace(uint8_t type) {
	switch(type) {
		case SubstitutionSchedeeType: schedule(); return;
		case AutomateSchedeeType: return;
	}
}