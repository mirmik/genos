#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <kernel/sysalloc.h>
#include <compiler.h>

#include <gxx/DList.h>
#include <gxx/HashTable.h>
#include <kernel/ipcstack/ipcstack.h>
#include <kernel/id/id.h>

class query {
public:
	dlist_head lnk;
	ipcstack *stack;

	qid_t sender;
	qid_t receiver;
};

class service {
public:
	hlist_node hlnk;
	gxx::DList<query, &query::lnk> queries;

	qid_t qid;

	qid_t getkey() const {
		return qid;
	}

	size_t hash(size_t seed) const {
		return qid ^ seed;
	}
};

__BEGIN_DECLS

static struct query * construct_query(struct ipcstack *stack, qid_t sender, qid_t receiver) {
	struct query* q = (struct query*) sysalloc(sizeof(query));
	dlist_init(&q->lnk);
	q->stack = stack;
	q->sender = sender;
	q->receiver = receiver;
	return q;
}

struct service* get_service(qid_t qid);

static void release_query(struct query *q) {
	sysfree(q);
}

static void __service_add_query(struct service *s, struct query *q) {
	s->queries.move_back(*q);
}

static int service_add_query(qid_t qid, struct query *q) {
	struct service *s = get_service(qid); 
	if (!s) return -1;
	__service_add_query(s, q);
	return 0;
}

__END_DECLS

#endif