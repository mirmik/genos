#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <kernel/service/service.h>
#include <kernel/sched/scheduler.h>
#include <kernel/panic.h>

class autom_schedee : public schedee {
protected:
	autom_schedee();
	~autom_schedee() override { }
public:
	gstack** 	ppstack;
	qid_t* 		retqid;
	
protected:
	virtual uint8_t execute() override {
		Routine();
		return SCHEDULE_REPEAT;
	}

	//virtual void invalidate() override {}
	virtual void Routine() = 0;
};

static int autom_schedee_send_query(struct service* s, struct gstack *stack, qid_t rqid) {
	dprln("autom_schedee_send_query");
	struct autom_schedee *asch = (autom_schedee*) s;
	struct query* q = construct_query(stack, rqid, asch->qid);

	assert(schedee_state_is(asch, SCHEDEE_STATE_RUN));
	
	int ret = kernel_transport_query(q);
	switch (ret) {
		//schedee переходит в состояние ожидания ответа.
		//функция release_query будет вызвана receive_answer
		case WAIT_ANSWER : schedee_set_state_wait(asch, SCHEDEE_BLOCKED_SEND); break;
		
		//если был возвращен FAST_ANSWER, функция release_query вызывается мгновенно
		case FAST_ANSWER : release_query(q); break;
	}
	return ret;
}

static int autom_schedee_add_query(struct service* s, struct query *q) {
	dprln("autom_schedee_add_query");
	struct autom_schedee *asch = (autom_schedee*) s;

	asch->queries.move_back(*q);
	if (schedee_state_is(asch, SCHEDEE_BLOCKED_RECEIVE) 
		&& (*asch->retqid == 0 || *asch->retqid == q->sender)) 
	{
		*asch->ppstack = q->stack;
		*asch->retqid = q->sender;

		schedee_set_state_wait(asch, SCHEDEE_STATE_RUN);
	}
	return 0;
}

static int autom_schedee_receive_query(struct service* s, qid_t sqid, gstack** ppstack, qid_t * retqid) {
	dprln("autom_schedee_receive_query");
	struct autom_schedee *asch = (autom_schedee*) s;
	query* q = kernel_service_find_query(s,sqid);

	if (q)  {
		*ppstack = q->stack;
		*retqid = q->sender;
		return FAST_RECEIVE;
	}
	else {
		asch->ppstack = ppstack;
		asch->retqid = retqid;
		*asch->retqid = sqid;
		schedee_set_state_wait(asch, SCHEDEE_BLOCKED_RECEIVE);
		return WAIT_RECEIVE;
	}
}

static int autom_schedee_reply_answer(struct service* s, struct query *q) {
	dprln("autom_schedee_reply_answer");
	kernel_service_unlink_query(s, q);
	kernel_transport_answer(q);
}

static int autom_schedee_receive_answer(struct service* s, struct query *q) {
	dprln("autom_schedee_receive_answer");
	struct autom_schedee *asch = (autom_schedee*) s;

	if (!schedee_state_is(asch, SCHEDEE_BLOCKED_SEND) || q->sender != asch->qid) 
		panic("REPLY TO UNSEND QUERY");

	schedee_set_state_run(asch);
	release_query(q);
}

static const struct service_operations autom_schedee_service_operations = {
	.send_query = autom_schedee_send_query,
	.add_query = autom_schedee_add_query,
	.receive_query = autom_schedee_receive_query,
	.reply_answer = autom_schedee_reply_answer,
	.receive_answer = autom_schedee_receive_answer,
};

autom_schedee::autom_schedee() { 
	hops = &autom_schedee_service_operations;
}

#endif