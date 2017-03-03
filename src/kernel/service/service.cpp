#include <kernel/service/service.h>
#include <kernel/service/table.h>
#include <kernel/sched/schedee.h>

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
	struct query *q = construct_query(stack, rqid, s->qid);
	
	if (!(s && s->hops && s->hops->send_query)) return ERROR_WRONG_SENDER;
	
	int ret = s->hops->send_query(s, q);
	release_query(q);
	return ret;
}

int kernel_transport_query(struct query* q) {
	struct service *r = kernel_get_service(q->receiver);
	if (!(r && r->hops)) return ERROR_WRONG_RECEIVER;
	return r->hops->receive_query(r, q);	
}

int kernel_send_query(qid_t receiver, struct gstack *stack) {
	return __kernel_send_query(stack, receiver, current_schedee());
}

//int kernel_finalize_query(struct query * q) {
//	release_query(q);
//}

qid_t kernel_get_new_qid() {
	service* __;
	while (service_hashtable.get(++__cur_qid, __))
		if (__cur_qid > __max_qid) __cur_qid = 0; 
	return __cur_qid;
}