#include <kernel/service/service.h>
#include <kernel/service/table.h>
#include <kernel/sched/schedee.h>
#include <kernel/panic.h>

qid_t __cur_qid;
qid_t __max_qid;

gxx::HashTable<service, &service::hlnk> service_hashtable;

struct service* kernel_get_service(qid_t qid) {
	service* s;
	bool res = service_hashtable.get(qid, s);
	return res ? s : NULL;
}

void kernel_service_table_init() {
	service_hashtable.setStrategy(gxx::hash_memstrat70);
}

void kernel_registry_stable_service(service* s, qid_t qid) {
	s->qid = qid;
	service_hashtable.put(*s);
}

qid_t kernel_registry_service(service* s) {
	s->qid = kernel_get_new_qid();
	service_hashtable.put(*s);
	return s->qid;
}

struct query * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender) {
	struct query* q = (struct query*) sysalloc(sizeof(query));
	dlist_init(&q->lnk);
	q->stack = stack;
	q->sender = sender;
	q->receiver = receiver;
	return q;
}

void release_query(struct query *q) {
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
	struct service *r = kernel_get_service(q->receiver);
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
	struct service *s = kernel_get_service(q->sender);
	
	//В случае, если за время обработки sender умер, или по каким-то другим
	//причинам не может быть найден, запрос будет уничтожен здесь.
	if (!(s && s->hops)) {
		release_query(q);
		return ERROR_REPLY;
	}	
	return s->hops->receive_answer(s, q);	
}

qid_t kernel_get_new_qid() {
	service* __;
	while (service_hashtable.get(++__cur_qid, __))
		if (__cur_qid > __max_qid) __cur_qid = 0; 
	return __cur_qid;
}

struct query * kernel_service_find_query(struct service * s, qid_t qid) {
	if (s->queries.empty()) return NULL;
	if (0 == qid) return &*s->queries.begin();
	auto q = gxx::find_if(s->queries.begin(), s->queries.end(), [qid](const query& q) -> bool{
		return q.sender == qid;
	});
	return q == s->queries.end() ? NULL : &*q;
}


void kernel_service_unlink_query(struct service* s, struct query* q) {
	dlist_del_init(&q->lnk);
}

////////SCHEDEE_API///////////
int kernel_send_query(qid_t receiver, struct gstack *stack) {
	return __kernel_send_query(stack, receiver, current_schedee());
}

int kernel_receive_query(qid_t sender, struct gstack **ppstack, qid_t * rqid) {
	service* s = current_schedee();
	return s->hops->receive_query(s, sender, ppstack, rqid);
}

int kernel_reply_query(qid_t receiver) {
	//dprln("kernel_reply_query");
	service* s = current_schedee();
	query* q = kernel_service_find_query(s, receiver);
	if (q) {
		return s->hops->reply_answer(s, q);
	} else {
		//Такого запроса не существует!!!
		return ERROR_REPLY;
	}
}