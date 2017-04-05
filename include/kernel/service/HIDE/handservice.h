#ifndef GENOS_HANDLER_SERVICE_H
#define GENOS_HANDLER_SERVICE_H

#include <kernel/service/service.h>

typedef

struct reactive_service {
	struct service;


};

int function_service_receive_query(struct service* s, struct query *q) {
	struct function_service *fsch = (function_service*) s;
	fsch->routine(q->stack);
	return FAST_ANSWER;
}

static const struct service_operations function_service_service_operations = {
	.send_query = NULL,
	.add_query = function_service_receive_query,
	.receive_query = NULL,
	.receive_answer = NULL,
};

static void function_service_init(struct function_service* fserv) { 
	fserv->hops = &function_service_service_operations;
}

#endif