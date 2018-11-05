#include <genos/schedule.h>
#include <genos/sched/schedee.h>
#include <genos/sched/impl/process.h>
#include <arch/startup.h>

void genos::displace() {
	//dprln("genos::displace");
	genos::schedee_manager.current_schedee->displace();
}

extern "C" void __displace__() {
	//dprln("__displace__");


	RESET_STACK(); 

	//context_print(&((genos::process_schedee*)genos::schedee_manager.current_schedee)->cntxt);

	//while(1);

	genos::schedule();	
}