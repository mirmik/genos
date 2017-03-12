#include <kernel/sched/schedee.h>
#include <kernel/sched/stubschedee.h>

int stub_schedee_send_query(struct service* ths, struct gstack *stack, qid_t rqid) {
	struct stub_schedee *sch = (struct stub_schedee*) ths;
	struct query* q = construct_query(stack, rqid, ths->qid);
	int ret = kernel_transport_query(q);
	switch (ret) {
		case WAIT_ANSWER : panic("stub schedee can`t waited");
		case FAST_ANSWER : return 0;
		default: return ret;
	}
	release_query(q);
}

const struct service_operations stub_schedee_service_operations = {
	.send_query = stub_schedee_send_query,
	.add_query = NULL,
	.receive_query = NULL,
	.reply_answer = NULL,
	.receive_answer = NULL,
};

const struct schedee_operations stub_schedee_schedee_operations = {
	.execute = NULL,
	.engage = NULL,
	.displace = NULL,
	.lastexit = NULL,
	.destructor = NULL,
};

struct stub_schedee * construct_stub_schedee() {
	struct stub_schedee * sch = (struct stub_schedee *) sysalloc(sizeof(struct stub_schedee));
	sch->sch.srvs.hops = &stub_schedee_service_operations;
	sch->sch.schops = &stub_schedee_schedee_operations;
	return sch;
}
