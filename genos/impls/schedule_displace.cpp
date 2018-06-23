#include <genos/schedule.h>
#include <arch/stack.h>

void genos::displace() {
	RESET_STACK(); 
	genos::schedule();
}