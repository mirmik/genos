#include <kernel/sched/substschedee.h>
#include <kernel/sched/scheduler.h>

context __initcntxt;
context* __current_context = &__initcntxt;

#define TODO_STACK_SIZE 500

static int subst_schedee_send_query(struct service* s, struct gstack *stack, qid_t rqid) {
	//dprln("subst_schedee_send_query");
	struct subst_schedee *ssch = (subst_schedee*) s;
	struct query* q = construct_query(stack, rqid, ssch->srvs.qid);

	assert(schedee_state_is(ssch, SCHEDEE_STATE_RUN));
	
	int ret = kernel_transport_query(q);
	switch (ret) {
		//schedee переходит в состояние ожидания ответа.
		//функция release_query будет вызвана receive_answer
		case WAIT_ANSWER : 
			schedee_set_state_wait(ssch, SCHEDEE_BLOCKED_SEND); 
			__displace__();
			break;
		
		//если был возвращен FAST_ANSWER, функция release_query вызывается мгновенно
		case FAST_ANSWER : release_query(q); break;
	}
	return ret;
}

static int subst_schedee_add_query(struct service* s, struct query *q) {
	//dprln("subst_schedee_add_query");
	struct subst_schedee *ssch = (subst_schedee*) s;

	dlist_move_back(&q->lnk, &ssch->srvs.qlist);
	if (schedee_state_is(ssch, SCHEDEE_BLOCKED_RECEIVE) 
		&& (*ssch->retqid == 0 || *ssch->retqid == q->sender)) 
	{
		*ssch->ppstack = q->stack;
		*ssch->retqid = q->sender;

		schedee_set_state_run(ssch);
	}
	return 0;
}

static int subst_schedee_receive_query(struct service* s, qid_t sqid, gstack** ppstack, qid_t * retqid) {
	//dprln("subst_schedee_receive_query");
	struct subst_schedee *ssch = (subst_schedee*) s;
	query* q = kernel_service_find_query(s,sqid);

	if (q)  {
		*ppstack = q->stack;
		*retqid = q->sender;
		return FAST_RECEIVE;
	}
	else {
		ssch->ppstack = ppstack;
		ssch->retqid = retqid;
		*ssch->retqid = sqid;
		schedee_set_state_wait(ssch, SCHEDEE_BLOCKED_RECEIVE);
		
		__displace__();

		return WAIT_RECEIVE;
	}
}

/*static int subst_schedee_reply_answer(struct service* s, struct query *q) {
	//dprln("subst_schedee_reply_answer");
	//kernel_service_unlink_query(s, q);
	dlist_del_init(&q->lnk);
	kernel_transport_answer(q);
}*/

static int subst_schedee_receive_answer(struct service* s, struct query *q) {
	//dprln("subst_schedee_receive_answer");
	struct subst_schedee *ssch = (subst_schedee*) s;

	if (!schedee_state_is(ssch, SCHEDEE_BLOCKED_SEND) || q->sender != ssch->srvs.qid) 
		panic("REPLY TO UNSEND QUERY");

	schedee_set_state_run(ssch);
	release_query(q);
}

static const struct service_operations subst_schedee_service_operations = {
	.send_query = subst_schedee_send_query,
	.add_query = subst_schedee_add_query,
	.receive_query = subst_schedee_receive_query,
	//.reply_answer = subst_schedee_reply_answer,
	.receive_answer = subst_schedee_receive_answer,
};

//subst_schedee::subst_schedee(void(*func)(void*), void* arg) {
//	void *stack = malloc(TODO_STACK_SIZE);
//	context_init(&ssch->cntxt, (uintptr_t)stack + TODO_STACK_SIZE - 1, func, arg, true);
//	hops = &subst_schedee_service_operations;
//}

uint8_t subst_schedee_execute(struct schedee* sch) {
	struct subst_schedee * ssch = (struct subst_schedee *) sch;

	if (__current_context == &ssch->cntxt) return SCHEDULE_RESUME;
	
	context* really_current = __current_context;
	__current_context = &ssch->cntxt;

	auto save = global_irqs_save();
	context_switch(really_current, &ssch->cntxt);
	global_irqs_restore(save);

	return SCHEDULE_RESUME;
}

uint8_t subst_schedee_engage(struct schedee* sch) {
	struct subst_schedee * ssch = (struct subst_schedee *) sch;
	__current_context = &ssch->cntxt;
	global_irqs_disable();
	context_load(&ssch->cntxt);
}

uint8_t subst_schedee_displace(struct schedee* sch) {
	struct subst_schedee * ssch = (struct subst_schedee *) sch;
	auto save = global_irqs_save();

	__schedee_plan__();
	if (planned_schedee() == sch) {
		set_planned_schedee(nullptr);
		global_irqs_restore(save);
		return 3;		
	}

	context_save_and_invoke_schedule(&ssch->cntxt);
	global_irqs_restore(save);
}

uint8_t subst_schedee_lastexit(struct schedee* sch) {
	struct subst_schedee * ssch = (struct subst_schedee *) sch;
	__displace__();
}

uint8_t subst_schedee_destructor(struct schedee* sch) {
	struct subst_schedee * ssch = (struct subst_schedee *) sch;
	free(ssch->stackptr);
}

const struct schedee_operations subst_schedee_schedee_operations = {
	.execute = subst_schedee_execute,
	.engage = subst_schedee_engage,
	.displace = subst_schedee_displace,
	.lastexit = subst_schedee_lastexit,
	.destructor = subst_schedee_destructor,
};

void subst_schedee_starter(void* _sch) {
	subst_schedee* sch = (subst_schedee*) _sch;
	sch->startpoint(sch->startarg);

	debug_print("schedee is finished");
	schedee_exit(sch);
	//panic("schedee is finished");
}

subst_schedee::subst_schedee(delegate<void, void*> dlg, void* arg) {
	schedee_init(this);
	startpoint = dlg;
	startarg = arg;
	stackptr = malloc(TODO_STACK_SIZE);
	context_init(&cntxt, (uintptr_t)stackptr + TODO_STACK_SIZE - 1, subst_schedee_starter, this, true);
	srvs.hops = &subst_schedee_service_operations;
	schops = &subst_schedee_schedee_operations;
}

