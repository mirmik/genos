#include <hal/arch.h>
#include <gxx/debug/dprint.h>
#include <sched/Scheduler.h>
#include <sched/AutomSchedee.h>

void func(void*) {
	dprln("KEKEKE");
}

int main() {
	arch_init();
	dprln("HelloWorld");


	Genos::AutomSchedee aut(func);
	Genos::Glue::registerSchedee(&aut);

	aut.run();

	while(1) Genos::Glue::schedule();
}