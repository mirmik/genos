#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <kernel/sysalloc.h>
#include <compiler.h>

#include <gxx/DList.h>
#include <gxx/HashTable.h>
#include <kernel/ipcstack/ipcstack.h>
#include <kernel/id/id.h>

#define WAIT_REPLY 0
#define FAST_REPLY 1
#define ERROR_REPLY 2

struct query {
public:
	dlist_head lnk;
	
	ipcstack *stack;
	int16_t errstat;

	qid_t sender;
	qid_t receiver;
};

struct service;

struct service_operations {
	uint8_t(*receive_query)(struct service* ths, struct query *q);
	void(*reply_answer)(struct service* ths, struct query *q);
};

struct client_operations {
	int(*send_query)(struct service* ths, struct query *q);
	void(*receive_answer)(struct service* ths, struct query *q);
};

class service {
public:
	hlist_node hlnk;
	gxx::DList<query, &query::lnk> queries;

	const struct service_operations* hops;
	const struct client_operations* cops;
	qid_t qid;

	//hashtable routine
	qid_t getkey() const {
		return qid;
	}

	size_t hash(size_t seed) const {
		return qid ^ seed;
	}
};

__BEGIN_DECLS

struct query * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender);
void release_query(struct query *q);

int __kernel_send_query(struct gstack *stack, qid_t rqid, qid_t sqid) ;

int kernel_send_query(qid_t receiver, struct gstack *stack);
int kernel_receive_query(struct query **ppq);
int kernel_reply_query(struct query *q);

int kernel_transport_query(struct query *q);

__END_DECLS

#endif