#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <kernel/service/service.h>
#include <kernel/sched/scheduler.h>
#include <genos/panic.h>

typedef void(*aschfunc_t)(void*);

struct autom_schedee {
	struct schedee sch;

	struct gstack** 	ppstack;
	qid_t* 				retqid;
	
	void (*routine)(void*);
	void* localstorage;
};

static int autom_schedee_send_query(struct service* s, struct gstack *stack, qid_t rqid) {
	debug_print("autom_schedee_send_query");dln();
	struct autom_schedee * asch = container_of((struct schedee*)s,struct autom_schedee,sch);
	struct query* q = construct_query(stack, rqid, asch->sch.srvs.qid);

	debug_printhex_ptr(asch);dln();
	assert(schedee_state_is(&asch->sch, SCHEDEE_STATE_RUN));
	
	int ret = kernel_transport_query(q);
	switch (ret) {
		//schedee переходит в состояние ожидания ответа.
		//функция release_query будет вызвана receive_answer
		case WAIT_ANSWER : schedee_set_state_wait(&asch->sch, SCHEDEE_BLOCKED_SEND); break;
		
		//если был возвращен FAST_ANSWER, функция release_query вызывается мгновенно
		case FAST_ANSWER : release_query(q); break;
	}
	return ret;
}

static int autom_schedee_add_query(struct service* s, struct query *q) {
	debug_print("autom_schedee_add_query");dln();
	struct autom_schedee * asch = container_of((struct schedee*)s,struct autom_schedee,sch);
	
	dlist_move_back(&q->lnk, &asch->sch.srvs.qlist);
	if (schedee_state_is(&asch->sch, SCHEDEE_BLOCKED_RECEIVE) 
		&& (*asch->retqid == 0 || *asch->retqid == q->sender)) 
	{
		*asch->ppstack = q->stack;
		*asch->retqid = q->sender;
	
		schedee_set_state_run(&asch->sch);
	}
	return 0;
}

static int autom_schedee_receive_query(struct service* s, qid_t sqid, struct gstack** ppstack, qid_t * retqid) {
	debug_print("autom_schedee_receive_query");dln();
	struct autom_schedee * asch = container_of((struct schedee*)s,struct autom_schedee,sch);
	struct query* q = kernel_service_find_query(s,sqid);

	if (q)  {
		*ppstack = q->stack;
		*retqid = q->sender;
		return FAST_RECEIVE;
	}
	else {
		asch->ppstack = ppstack;
		asch->retqid = retqid;
		*asch->retqid = sqid;
		schedee_set_state_wait(&asch->sch, SCHEDEE_BLOCKED_RECEIVE);
		return WAIT_RECEIVE;
	}
}

/*static int autom_schedee_reply_answer(struct service* s, struct query *q) {
	debug_print("autom_schedee_reply_answer");dln();
	dlist_del_init(&q->lnk);
	kernel_transport_answer(q);
}*/

static int autom_schedee_receive_answer(struct service* s, struct query *q) {
	debug_print("autom_schedee_receive_answer");dln();
	struct autom_schedee * asch = container_of((struct schedee*)s,struct autom_schedee,sch);
	
	if (!schedee_state_is(&asch->sch, SCHEDEE_BLOCKED_SEND) || q->sender != asch->sch.srvs.qid) 
		panic("REPLY TO UNSEND QUERY");

	schedee_set_state_run(&asch->sch);
	release_query(q);
}

static const struct service_operations autom_schedee_service_operations = {
	.send_query = autom_schedee_send_query,
	.add_query = autom_schedee_add_query,
	.receive_query = autom_schedee_receive_query,
	//.reply_answer = autom_schedee_reply_answer,
	.receive_answer = autom_schedee_receive_answer,
};



uint8_t autom_schedee_execute(struct schedee* sch) {
	struct autom_schedee * asch = container_of(sch,struct autom_schedee,sch);
	asch->routine(asch->localstorage);
	return SCHEDULE_REPEAT;
}

uint8_t autom_schedee_engage(struct schedee* sch) {
	struct autom_schedee * asch = container_of(sch,struct autom_schedee,sch);
	asch->routine(asch->localstorage);
}

uint8_t autom_schedee_displace(struct schedee* sch) {}
uint8_t autom_schedee_lastexit(struct schedee* sch) {}
uint8_t autom_schedee_destructor(struct schedee* sch) {}

const struct schedee_operations autom_schedee_schedee_operations = {
	.execute = autom_schedee_execute,
	.engage = autom_schedee_engage,
	.displace = autom_schedee_displace,
	.lastexit = autom_schedee_lastexit,
	.destructor = autom_schedee_destructor,
};

struct schedee* construct_autom_schedee(void(*routine)(void*), void* localstorage) { 
	struct autom_schedee* asch = (struct autom_schedee*) sysalloc(sizeof(struct autom_schedee));
	
	asch->routine = routine;
	asch->localstorage = localstorage;

	schedee_init(&asch->sch, &autom_schedee_schedee_operations, &autom_schedee_service_operations);
	return &asch->sch;
}

#if defined __cplusplus
#include <genos/sigslot/delegate.h>
struct schedee* construct_autom_schedee(const fastdelegate<void>& dlg) {
	return construct_autom_schedee(dlg.extfunction, dlg.object);
}
#endif

#endif