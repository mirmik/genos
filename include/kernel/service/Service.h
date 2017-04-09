#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <kernel/service/MsgTag.h>
#include <datastruct/hlist_head.h>
#include <kernel/id/id.h>
#include <kernel/csection.h>

#define WAIT_REPLY 0
#define FAST_REPLY 1
#define ERROR_REPLY -1

namespace Genos {
	
	class Service {
	public:
		struct hlist_node hlnk;
		qid_t qid;
	public:
		virtual int8_t receiveMessageHandler(MessageHeader& msg) = 0;
	
		//hashtable support
		static qid_t& getkey(Service& srvs) { return srvs.qid; }
	};

	class PostBoxService : public Service {
		MessageList queries;

	public:
		int8_t receiveMessageHandler(MessageHeader& msg) {
			critical_section_enter();
			queries.move_back(msg);
			critical_section_leave();
		}

		//virtual MsgTag* receiveMessage();
	};
}



/*
msgtag_t * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender);
void release_query(msgtag_t *q);

static int service_unlink_query(service_t* srvs, msgtag_t* q) {
	dlist_del_init(&q->lnk);
}

int kernel_send_query(qid_t receiver, struct gstack *stack);
int kernel_receive_query(qid_t sender, struct ipcstack **ppstack, qid_t * retqid);
int kernel_reply_query(qid_t receiver);
	
int kernel_transport_query(msgtag_t *q);
int kernel_transport_answer(msgtag_t *q);*/

#endif