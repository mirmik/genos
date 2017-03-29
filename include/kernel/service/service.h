#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <compiler.h>
#include <datastruct/hlist_head.h>
#include <kernel/sysalloc.h>
#include <kernel/service/errno.h>
#include <kernel/ipcstack/ipcstack.h>
#include <kernel/service/msgtag.h>
#include <kernel/id/id.h>

#define WAIT_ANSWER 0
#define FAST_ANSWER 1

#define WAIT_RECEIVE 0
#define FAST_RECEIVE 1

#define ERROR_REPLY -1

struct service;
typedef struct service service_t;

struct service_operations {
	int(*send_query)(struct service* ths, struct gstack* stack, qid_t rqid);
	int(*new_query)(struct service* ths, msgtag_t *q);
	int (*receive_query)(struct service* ths, qid_t q, struct gstack** ppstack, qid_t *retqid);
	int(*reply_answer)(struct service* ths, msgtag_t *q);
	int(*receive_answer)(struct service* ths, msgtag_t *q);
};

struct service {
	struct hlist_node hlnk;
	//struct dlist_head qlist;

	const struct service_operations* ops;
	qid_t qid;
};

__BEGIN_DECLS

static void service_init(struct service* srvs, const struct service_operations* ops) {
	//hlist_node_init(&srvs->hlnk);
	srvs->ops = ops;
}

msgtag_t * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender);
void release_query(msgtag_t *q);

static int service_unlink_query(service_t* srvs, msgtag_t* q) {
	dlist_del_init(&q->lnk);
}

__END_DECLS

//__BEGIN_DECLS

//namespace genos {

//	const char* UnsupportedServiceOperation = "UnsupportedServiceOperation";

/*	class service {
	public:
		struct hlist_node hlnk;
		qid_t qid;

	public:
		virtual int send_query_handler (msgtag_t *q) {
			panic(UnsupportedServiceOperation);
		}

		virtual int receive_query_handler (msgtag_t *q) {
			panic(UnsupportedServiceOperation);
		}

		virtual int reply_answer_handler (msgtag_t *q) {
			panic(UnsupportedServiceOperation);
		}

		virtual int receive_answer_handler (msgtag_t *q) {
			panic(UnsupportedServiceOperation);
		}
	};*/

		
int kernel_send_query(qid_t receiver, struct gstack *stack);
int kernel_receive_query(qid_t sender, struct ipcstack **ppstack, qid_t * retqid);
int kernel_reply_query(qid_t receiver);
	
int kernel_transport_query(msgtag_t *q);
int kernel_transport_answer(msgtag_t *q);

//}

//msgtag_t* kernel_service_find_query(struct service *s, qid_t qid);
//void kernel_service_unlink_query(struct service* s, msgtag_t* q);

//__END_DECLS

#endif