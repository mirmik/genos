#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

#include <kernel/sched/schedee.h>
#include <stdbool.h>

#ifndef PRIORITY_TOTAL
#	define PRIORITY_TOTAL 6
#endif
//__BEGIN_DECLS

namespace genos {

	struct schedee* planned_schedee();
	void set_planned_schedee(struct schedee* sch);
	
	void genos_idle();
	
	void scheduler_free(void*);
	
	
	void __schedee_plan__();
	void __schedule__();
	void __displace__();
	
	void kernel_stub_schedee_init();
	void scheduler_init();

}
//__END_DECLS

#endif