#include <genos/autom_schedee.h>

void autom_schedee_execute(struct schedee * sch) 
{
	struct autom_schedee * asch = mcast_out(sch, struct autom_schedee, sch);

	asch->func(asch->priv, &asch->state);
}

void autom_schedee_finalize(struct schedee * sch) 
{
	struct autom_schedee * asch = mcast_out(sch, struct autom_schedee, sch);
}

struct schedee_operations autom_schedee_ops = {
	.execute = autom_schedee_execute,
	.finalize = autom_schedee_finalize
};

void autom_schedee_init(
	struct autom_schedee * sch, 
	autom_schedee_func_t foo, 
	void * privdata) 
{
	schedee_init(&sch->sch, SCHEDEE_PRIORITY_TOTAL - 1, 0, & autom_schedee_ops);

	sch->state = 0;
	sch->func = foo;
	sch->priv = privdata;
}