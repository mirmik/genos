#include <genos/sched/process.h>
#include <genos/sched/impl/process.h>
#include <genos/schedule.h>

#include <gxx/panic.h>
#include <gxx/debug/delay.h>

#include <gxx/util/iteration_counter.h>

void starter(void* arg) {
	genos::process_schedee* sch = (genos::process_schedee*) arg;
	sch->func(sch->arg);
	genos::schedee_manager.final(*sch);
	__displace__();
}

genos::schedee_ptr genos::create_process(void(*func)(void*), void* arg, gxx::buffer zone) {
	genos::process_schedee* sch = new genos::process_schedee;
	dlist_init(&sch->lnk);
	sch->zone = zone;
	sch->func = func;
	sch->arg = arg;
	context_init(&sch->cntxt, (uintptr_t)(zone.data() + zone.size() - 2), starter, sch, true);
	//context_print(&sch->cntxt);
	schedee_manager.run(*sch);
	return sch;
}

void genos::process_schedee::execute() {
	//dprln("genos::process_schedee::execute");
	//context_print(&cntxt);
	//while(1);
	context_load(&cntxt);
}

void genos::process_schedee::finalize() {
	delete this;
}

extern "C" void genos_context_displace(struct context*) __attribute__((noinline));
extern "C" void genos_context_displace(struct context*);
void genos::process_schedee::displace() {
	genos_context_displace(&cntxt);
}
