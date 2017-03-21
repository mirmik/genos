#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <compiler.h>
#include <datastruct/hlist_head.h>
#include <kernel/sysalloc.h>
#include <kernel/service/errno.h>
#include <kernel/ipcstack/ipcstack.h>
#include <kernel/service/query.h>
#include <kernel/id/id.h>

#define WAIT_ANSWER 0
#define FAST_ANSWER 1

#define WAIT_RECEIVE 0
#define FAST_RECEIVE 1

#define ERROR_REPLY -1

struct service;

struct service_operations {
	int(*send_query)(struct service* ths, struct gstack* stack, qid_t rqid);
	int(*add_query)(struct service* ths, struct query *q);
	int (*receive_query)(struct service* ths, qid_t q, struct gstack** ppstack, qid_t *retqid);
	//int(*reply_answer)(struct service* ths, struct query *q);
	int(*receive_answer)(struct service* ths, struct query *q);
};

struct service {
	struct hlist_node hlnk;
	struct dlist_head qlist;

	const struct service_operations* hops;
	qid_t qid;
};

__BEGIN_DECLS

void service_init(struct service*);

struct query * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender);
void release_query(struct query *q);

int kernel_send_query(qid_t receiver, struct gstack *stack);
int kernel_receive_query(qid_t sender, struct ipcstack **ppstack, qid_t * retqid);
int kernel_reply_query(qid_t receiver);

int kernel_transport_query(struct query *q);
int kernel_transport_answer(struct query *q);

struct query* kernel_service_find_query(struct service *s, qid_t qid);
void kernel_service_unlink_query(struct service* s, struct query* q);

__END_DECLS

#endif