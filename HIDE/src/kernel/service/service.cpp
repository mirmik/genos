#include <kernel/service/service.h>
#include <kernel/service/table.h>
//#include <kernel/sched/schedee.h>
#include <kernel/panic.h>

#include <debug/dprint.h>

//namespace genos {

/*	struct query * construct_query(struct ipcstack *stack, qid_t receiver, qid_t sender) {
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
	
	int __kernel_send_query(struct gstack *stack, qid_t rqid, genos::service * s) {
		//struct query *q = construct_query(stack, rqid, s->qid);
	
		if (s == nullptr) panic("incorrect sender schedee");
		
		int ret = s->srvs_send_query(stack, rqid);
		return ret;
	}
	
	//Функция занимается транспортировкой запроса между сервисами.
	//В случае, если receiver не может быть обнаружен, возвращается 
	//Должна вызываться из sender->send_query.
	int kernel_transport_query(struct query* q) {
		genos::service *r = genos::get_service(q->receiver);
		if (r == nullptr) {
			stack_pop_all(q->stack);
			stack_set_error(q->stack, ERROR_WRONG_RECEIVER, ERROR_WRONG_RECEIVER_MSG);
			return FAST_ANSWER;
		}
		return r->srvs_add_query(q);	
	}
	
	//Функция занимается транспортировкой ответа вызывающему сервису.
	//Должна вызываться из receiver->reply_answer.
	int kernel_transport_answer(struct query* q) {
		genos::service *s = get_service(q->sender);
		
		//В случае, если за время обработки sender умер, или по каким-то другим
		//причинам не может быть найден, запрос будет уничтожен здесь.
		if (s == nullptr) {
			release_query(q);
			return ERROR_REPLY;
		}	
		return s->srvs_receive_answer(q);	
	}
	/*
	struct query * kernel_service_find_query(genos::service * s, qid_t qid) {
		struct query* it;
	
		if (dlist_empty(&s->qlist)) return NULL;
		if (0 == qid) return dlist_first_entry(&s->qlist, struct query, lnk);
	
		dlist_for_each_entry(it, &s->qlist, lnk) {
			if (it->sender == qid) return it;
		}
		return NULL;
	}*/
	
	/*
	void kernel_service_unlink_query(genos::service* s, struct query* q) {
		dlist_del_init(&q->lnk);
	}*/
	
	////////SCHEDEE_API///////////
/*	int kernel_send_query(qid_t receiver, struct gstack *stack) {
		return __kernel_send_query(stack, receiver, current_schedee());
	}
	
	int kernel_receive_query(qid_t sender, struct gstack **ppstack, qid_t * rqid) {
		genos::service* s = current_schedee();
		return s->srvs_receive_query(sender, ppstack, rqid);
	}
	
	int __kernel_reply_query(struct query* q) {
		dlist_del_init(&q->lnk);
		kernel_transport_answer(q);
	}
	
	int kernel_reply_query(qid_t receiver) {
		/*genos::service* s = &current_schedee()->srvs;
		struct query* q = kernel_service_find_query(s, receiver);
		if (q) {
			__kernel_reply_query(q);
		} else {
			//Такого запроса не существует!!!
			return ERROR_REPLY;
		}*/
//	}

//}