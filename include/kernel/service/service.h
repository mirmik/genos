#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <kernel/service/errno.h>

#include <kernel/sysalloc.h>
#include <compiler.h>

#include <gxx/DList.h>
#include <gxx/HashTable.h>
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
	int(*reply_answer)(struct service* ths, struct query *q);
	int(*receive_answer)(struct service* ths, struct query *q);
};

class service {
public:
	hlist_node hlnk;
	gxx::DList<query, &query::lnk> queries;

	const struct service_operations* hops;
	qid_t qid;

	//hashtable routine
	qid_t getkey() const {
		return qid;
	}

	size_t hash(size_t seed) const {
		return qid ^ seed;
	}

	~service () { hlist_del(&hlnk); }
};

__BEGIN_DECLS

struct query * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender);
void release_query(struct query *q);

int __kernel_send_query(struct gstack *stack, qid_t rqid, qid_t sqid) ;

int kernel_send_query(qid_t receiver, struct gstack *stack);
int kernel_receive_query(qid_t sender, struct ipcstack **ppstack, qid_t * retqid);
int kernel_reply_query(qid_t receiver);

int kernel_transport_query(struct query *q);
int kernel_transport_answer(struct query *q);

query* kernel_service_find_query(struct service *s, qid_t qid);
void kernel_service_unlink_query(struct service* s, struct query* q);

__END_DECLS

#endif