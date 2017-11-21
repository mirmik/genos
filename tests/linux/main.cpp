#include <genos/banner.h>
#include <gxx/print.h>
#include <genos/tasklet.h>
#include <genos/sched/autom_schedee.h>
#include <gxx/util/iteration_counter.h>

#include <genos/schedule.h>

void hello() {
	gxx::println("HelloWorld");
}
genos::autom_schedee asch(hello);

void hello2() {
	gxx::println("HelloWorld2");
	do_after_iteration(3) {
		genos::schedee_manager.stop(asch);
		genos::schedee_manager.stop(genos::schedee_manager.current());
	}
}
genos::autom_schedee asch2(hello2);

int main() {
	gxx::debug_ostream dout;
	genos::print_banner(dout);
	genos::print_about(dout);

	genos::schedee_manager.run(asch);
	genos::schedee_manager.run(asch2);

	while(true) {
		genos::schedule();
	}
}

void genos::schedule() {
	genos::tasklet_manager.execute();
	genos::schedee_manager.execute();
	if (genos::schedee_manager.total_runned() == 0) exit(0);
}