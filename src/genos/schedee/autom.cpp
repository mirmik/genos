#include <genos/schedee/autom.h>
#include <igris/util/bug.h>

/*struct schedee * create_autom_schedee(void* (*task) (void*, int*), void * arg)
{
	struct autom_schedee * sch = (struct autom_schedee *)
	                             malloc(sizeof(struct autom_schedee));

	schedee_init(sch, 0, &autom_schedee_op);

	sch->task = task;
	sch->arg = arg;
	sch->state = 0;

	sch->flag.can_displace = 1;
	sch->flag.dynamic = true;

	return sch;
}*/

void genos::autom_schedee::execute()
{
	task(arg, &state);
}

int genos::autom_schedee::displace()
{
	flag.runned = 0;
	__context_displace_vector__();
}

void genos::autom_schedee::finalize()
{
	if (flag.dynamic)
		BUG();
}
