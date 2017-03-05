#ifndef GENOS_STUB_SCHEDEE_H
#define GENOS_STUB_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <kernel/panic.h>

class stub_schedee : public schedee {
public:
	uint8_t execute() override {
		panic("stubschedee_execute");
	}

	//void invalidate() {
	//	panic("stubschedee_invalidate");
	//}

	stub_schedee();
};

static int stub_schedee_send_query(struct service* ths, struct gstack *stack, qid_t rqid) {
	struct stub_schedee *sch = (stub_schedee*) ths;
	struct query* q = construct_query(stack, rqid, ths->qid);
	int ret = kernel_transport_query(q);
	switch (ret) {
		case WAIT_ANSWER : panic("stub schedee can`t waited");
		case FAST_ANSWER : return 0;
		default: return ret;
	}
	release_query(q);
}

static const struct service_operations stub_schedee_service_operations = {
	.send_query = stub_schedee_send_query,
	.add_query = NULL,
	.receive_query = NULL,
	.reply_answer = NULL,
	.receive_answer = NULL,
};

#endif