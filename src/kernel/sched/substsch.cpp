#include <kernel/sched/substschedee.h>
#include <kernel/sched/scheduler.h>

context __initcntxt;
context* __current_context = &__initcntxt;

#define TODO_STACK_SIZE 500

static int subst_schedee_send_query(struct service* s, struct gstack *stack, qid_t rqid) {
	//dprln("subst_schedee_send_query");
	struct subst_schedee *ssch = (subst_schedee*) s;
	struct query* q = construct_query(stack, rqid, ssch->qid);

	assert(schedee_state_is(ssch, SCHEDEE_STATE_RUN));
	
	int ret = kernel_transport_query(q);
	switch (ret) {
		//schedee переходит в состояние ожидания ответа.
		//функция release_query будет вызвана receive_answer
		case WAIT_ANSWER : 
			schedee_set_state_wait(ssch, SCHEDEE_BLOCKED_SEND); 
			__schedule__();
			break;
		
		//если был возвращен FAST_ANSWER, функция release_query вызывается мгновенно
		case FAST_ANSWER : release_query(q); break;
	}
	return ret;
}

static int subst_schedee_add_query(struct service* s, struct query *q) {
	//dprln("subst_schedee_add_query");
	struct subst_schedee *ssch = (subst_schedee*) s;

	ssch->queries.move_back(*q);
	if (schedee_state_is(ssch, SCHEDEE_BLOCKED_RECEIVE) 
		&& (*ssch->retqid == 0 || *ssch->retqid == q->sender)) 
	{
		*ssch->ppstack = q->stack;
		*ssch->retqid = q->sender;

		schedee_set_state_wait(ssch, SCHEDEE_STATE_RUN);
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
		__schedule__();
		return WAIT_RECEIVE;
	}
}

static int subst_schedee_reply_answer(struct service* s, struct query *q) {
	//dprln("subst_schedee_reply_answer");
	kernel_service_unlink_query(s, q);
	kernel_transport_answer(q);
}

static int subst_schedee_receive_answer(struct service* s, struct query *q) {
	//dprln("subst_schedee_receive_answer");
	struct subst_schedee *ssch = (subst_schedee*) s;

	if (!schedee_state_is(ssch, SCHEDEE_BLOCKED_SEND) || q->sender != ssch->qid) 
		panic("REPLY TO UNSEND QUERY");

	schedee_set_state_run(ssch);
	release_query(q);
}

static const struct service_operations subst_schedee_service_operations = {
	.send_query = subst_schedee_send_query,
	.add_query = subst_schedee_add_query,
	.receive_query = subst_schedee_receive_query,
	.reply_answer = subst_schedee_reply_answer,
	.receive_answer = subst_schedee_receive_answer,
};

subst_schedee::subst_schedee(void(*func)(void*), void* arg) {
	void *stack = malloc(TODO_STACK_SIZE);
	context_init(&cntxt, (uintptr_t)stack + TODO_STACK_SIZE - 1, func, arg, true);
	hops = &subst_schedee_service_operations;
}

subst_schedee::~subst_schedee() {}

uint8_t subst_schedee::execute() {
	if (__current_context == &cntxt) return SCHEDULE_RESUME;
	
	context* really_current = __current_context;
	__current_context = &cntxt;

	context_switch(really_current, &cntxt);
	return SCHEDULE_RESUME;
}