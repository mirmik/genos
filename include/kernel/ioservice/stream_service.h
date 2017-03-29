#ifndef GENOS_IOSERVICE_H
#define GENOS_IOSERVICE_H

#include <datastruct/dlist_head.h>
#include <util/retype.h>

struct stream_service;

struct stream_service_operations {
	int(*write)(struct stream_service*, const char*, size_t);
	int(*read)(struct stream_service*, const char*, size_t);
	int(*avail)(struct stream_service*);
};

struct stream_service {
	struct multi_function_service mfsrvs;

	struct dlist_head wq;
	struct dlist_head rq;

	const stream_service_operations * ops;
};

__BEGIN_DECLS

void stream_service_invoke_write_query(mf_service_t* s) {
	char * str; 
	size_t sz; 
	


//	stack_get_string_slice(q->stack, 2, &str, &sz, NULL);
//	if (sz == 0) service_reply_query(&s->srvs, q);
		
	//else if (ss->ops->write(ss, str, sz) == FAST_REPLY);
}

void stream_service_write_query(mf_service_t* s, query_t* q) {
	struct ipcstack * stack = q->stack;
	if (!stack_is_int32(stack, 1) || !stack_is_string(stack, 2)) {
		stack_set_error(stack, -4, "WrongArguments");
	//	service_reply_query(q);
		return;
	}

	RETYPE(stream_service*, ss, s);

	irqstate_t save = global_irq_save();
	
	dlist_move_back(&q->lnk, &ss->wq);
	
	//Если данный запрос оказался первым в списке,
	//инициируем выполнение.
//	if (dlist_first(&ss->wq) == &q->lnk) {
//		stream_service_invoke_write_query(s);
//	} 

	global_irq_restore(save);
}

void stream_service_read_query(mf_service_t* s, query_t* q) {
	RETYPE(stream_service*, ss, s);
	dlist_move_back(&q->lnk, &ss->wq);
}

void stream_service_flush_query(mf_service_t* s, query_t* q) {
	RETYPE(stream_service*, ss, s);
	dlist_move_back(&q->lnk, &ss->wq);
}

void stream_service_avail_query(mf_service_t* s, query_t* q) {
	RETYPE(stream_service*, ss, s);
	dlist_move_back(&q->lnk, &ss->wq);
}

const msrvfunc_t stream_service_functions[] {
	stream_service_write_query,
	stream_service_read_query,
	stream_service_flush_query,
	stream_service_avail_query,	
};

void stream_service_init(struct stream_service* iosrv) {
	multi_function_service_init(&iosrv->mfsrvs, stream_service_functions, 
		sizeof(stream_service_functions) / sizeof(msrvfunc_t));
}

__END_DECLS

#endif