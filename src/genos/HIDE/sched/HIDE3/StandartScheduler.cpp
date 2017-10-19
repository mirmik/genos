#include <sched/Scheduler.h>

void genos::sched::displace(uint8_t type) {
	switch(type) {
		case substtype: schedule(); return;
		case automtype: return;
	}
}