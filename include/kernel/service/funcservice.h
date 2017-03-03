#ifndef GENOS_FUNCTION_SERVICE_H
#define GENOS_FUNCTION_SERVICE_H

#include <kernel/service/service.h>

class function_service : public service {
public:
	function_service();
	virtual void routine(struct gstack* stack) = 0;
};

uint8_t function_service_receive_query(struct service* s, struct query *q) {
	struct function_service *fsch = (function_service*) s;
	fsch->routine(q->stack);
	return FAST_REPLY;
}

static const struct service_operations function_service_service_operations = {
	.receive_query = function_service_receive_query,
	do_nothing
};

function_service::function_service() { 
	hops = &function_service_service_operations;
}

#endif