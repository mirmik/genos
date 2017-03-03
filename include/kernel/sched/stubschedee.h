#ifndef GENOS_STUB_SCHEDEE_H
#define GENOS_STUB_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <kernel/panic.h>

class stub_schedee : public schedee {
public:
	void execute() {
		panic("stubschedee_execute");
	}

	void invalidate() {
		panic("stubschedee_invalidate");
	}

	stub_schedee();
};

static int stub_schedee_send_query(struct service* ths, struct query *q) {
	struct stub_schedee *sch = (stub_schedee*) ths;
	int ret = kernel_transport_query(q);
	switch (ret) {
		case WAIT_REPLY : panic("stub schedee can`t waited");
		case FAST_REPLY : return 0;
		default: return ret;
	}
}

static const struct service_operations stub_schedee_service_operations = {
	.send_query = stub_schedee_send_query,
	.receive_query = NULL,
	.reply_answer = NULL,
	.receive_answer = NULL,
};

#endif