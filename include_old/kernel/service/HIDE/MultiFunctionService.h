#ifndef GENOS_MULTI_FUNCTION_SERVICE_H
#define GENOS_MULTI_FUNCTION_SERVICE_H

#include <kernel/service/service.h>
#include <kernel/ipcstack/ipcstack.h>
#include <gxx/buffer.h>

struct multi_function_service;
typedef struct multi_function_service mf_service_t;
typedef void(*msrvfunc_t)(mf_service_t*, query_t*);

struct multi_function_service {
	struct service 	srvs;
	const msrvfunc_t * 	funcs;
	size_t 			fsize;
};

static int mf_new_query_handler (service_t* srvs, query_t *q) {
	mf_service_t * msrvs = (mf_service_t*) srvs;

	ipcstack* stack = q->stack;
	int num = stack_get_int32(stack, 1, NULL);
	if (num >= msrvs->fsize || num < 0) {
		stack_set_error(stack, -3, "IncorectFuncNumber");
		return FAST_RECEIVE;
	}
	msrvs->funcs[num](msrvs, q);
}

const struct service_operations multi_function_service_operations = {
	.send_query = NULL,
	.new_query = mf_new_query_handler,
	.receive_query = NULL,
	.reply_answer = service_unlink_query,
	.receive_answer = NULL,
};

static void multi_function_service_init(mf_service_t* msrvs, const msrvfunc_t* funcs, size_t fsize) {
	service_init(&msrvs->srvs, &multi_function_service_operations);
	msrvs->funcs = funcs;
	msrvs->fsize = fsize;
}

/*namespace genos {

	class multi_function_service;

	using msrvfunc_t = void(*)(multi_function_service*, struct query*); 

	class multi_function_service : public service {
		gxx::slice<msrvfunc_t> funcs;

	public:
		multi_function_service(msrvfunc_t* buf, size_t sz) 
			: funcs(buf, sz) {};


		int receive_query_handler (struct query *q) {
			ipcstack* stack = q->stack;
			int num = stack_get_int32(stack, 1, NULL);
			if (num >= funcs.size() || num < 0) {
				stack_set_error(stack, -3, "IncorectFuncNumber");
				return FAST_RECEIVE;
			}
			funcs[num](this, q);

		}

	};

}*/



#endif