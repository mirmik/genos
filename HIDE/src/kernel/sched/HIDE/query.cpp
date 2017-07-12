#include "util/retype.h"
#include "kernel/sched/query.h"
#include "kernel/sched/sched.h"
#include "genos/datastruct/dlist_head.h"
#include "kernel/sched/virtee.h"
#include "kernel/sched/schedee.h"
#include "genos/panic.h"
#include <gxx/debug/dprint.h>

/*
void schedee_onquery(struct virtee* vee, struct query* qry) {
	debug_print("TODO/query\r\n");
	RETYPE(struct schedee*, sch, vee);
	dlist_add_prev(&qry->lnk, &sch->qsocket);
}

void schedee_onreceive(struct virtee* vee) {
	debug_print("TODOreceive\r\n");
	RETYPE(struct schedee*, sch, vee);
	if (schedee_is_blocked_query(sch))
		schedee_set_wait(sch, SCHEDEE_BLOCKED_RECEIVE);
	else panic("receive without query");
}

void schedee_onreply(struct virtee* vee) {
	debug_print("TODO/reply\r\n");
	RETYPE(struct schedee*, sch, vee);
	if (schedee_is_blocked_receive(sch))
		schedee_set_ready(sch, 1);
	else panic("reply without receive");
}

struct virtee_operations schedee_virtee_operations = {
	.onquery = schedee_onquery,
	.onreceive = schedee_onreceive,
	.onreply = schedee_onreply,
};*/
/*
void Query(id_t id, const void* smsg, size_t slen,
		void* rmsg, size_t rlen) {
	debug_print("Query\r\n");
	struct schedee* sch = schedee_current_get();
	sch->ops->query(id,smsg,slen,rmsg,rlen);
}

void Receive(void* msg, size_t len) {
	debug_print("Receive\r\n");
	struct schedee* sch = schedee_current_get();
	sch->ops->receive(msg,len);
}

void Reply(const void* msg, size_t len) {
	debug_print("Reply\r\n");
	struct schedee* sch = schedee_current_get();
	sch->ops->reply(msg,len);
}*/

id_t QRRret() {
	struct schedee* sch = scheduler::get_current();
	return sch->qret;	
}