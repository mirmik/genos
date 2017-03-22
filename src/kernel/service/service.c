#include <kernel/service/service.h>
#include <kernel/service/table.h>
#include <kernel/sched/schedee.h>
#include <kernel/panic.h>

#include <debug/dprint.h>

void service_init(struct service* srvs) {
	//dlist_init(&srvs->qlist);
};

struct query * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender) {
	//debug_print("construct_query\r\n");
	struct query* q = (struct query*) sysalloc(sizeof(struct query));
	dlist_init(&q->lnk);
	q->stack = stack;
	q->sender = sender;
	q->receiver = receiver;
	return q;
}

void release_query(struct query *q) {
	//debug_print("release_query\r\n");
	sysfree(q);
}

int __kernel_send_query(struct gstack *stack, qid_t rqid, struct service * s) {
	//struct query *q = construct_query(stack, rqid, s->qid);

	if (!(s && s->hops && s->hops->send_query)) panic("incorrect sender schedee");
	
	int ret = s->hops->send_query(s, stack, rqid);
	return ret;
}

//Функция занимается транспортировкой запроса между сервисами.
//В случае, если receiver не может быть обнаружен, возвращается 
//Должна вызываться из sender->send_query.
int kernel_transport_query(struct query* q) {
	struct service *r = get_service(q->receiver);
	if (!(r && r->hops)) {
		stack_pop_all(q->stack);
		stack_set_error(q->stack, ERROR_WRONG_RECEIVER, ERROR_WRONG_RECEIVER_MSG);
		return FAST_ANSWER;
	}
	return r->hops->add_query(r, q);	
}

//Функция занимается транспортировкой ответа вызывающему сервису.
//Должна вызываться из receiver->reply_answer.
int kernel_transport_answer(struct query* q) {
	struct service *s = get_service(q->sender);
	
	//В случае, если за время обработки sender умер, или по каким-то другим
	//причинам не может быть найден, запрос будет уничтожен здесь.
	if (!(s && s->hops)) {
		release_query(q);
		return ERROR_REPLY;
	}	
	return s->hops->receive_answer(s, q);	
}
/*
struct query * kernel_service_find_query(struct service * s, qid_t qid) {
	struct query* it;

	if (dlist_empty(&s->qlist)) return NULL;
	if (0 == qid) return dlist_first_entry(&s->qlist, struct query, lnk);

	dlist_for_each_entry(it, &s->qlist, lnk) {
		if (it->sender == qid) return it;
	}
	return NULL;
}*/

/*
void kernel_service_unlink_query(struct service* s, struct query* q) {
	dlist_del_init(&q->lnk);
}*/

////////SCHEDEE_API///////////
int kernel_send_query(qid_t receiver, struct gstack *stack) {
	return __kernel_send_query(stack, receiver, &current_schedee()->srvs);
}

int kernel_receive_query(qid_t sender, struct gstack **ppstack, qid_t * rqid) {
	struct service* s = &current_schedee()->srvs;
	return s->hops->receive_query(s, sender, ppstack, rqid);
}

int __kernel_reply_query(struct query* q) {
	dlist_del_init(&q->lnk);
	kernel_transport_answer(q);
}

int kernel_reply_query(qid_t receiver) {
	struct service* s = &current_schedee()->srvs;
	struct query* q = kernel_service_find_query(s, receiver);
	if (q) {
		__kernel_reply_query(q);
	} else {
		//Такого запроса не существует!!!
		return ERROR_REPLY;
	}
}