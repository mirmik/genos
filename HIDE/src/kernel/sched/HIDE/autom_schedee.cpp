#include "kernel/sched/autom_schedee.h"
#include "kernel/sched/schedee.h"
#include "kernel/sched/sched.h"
#include "kernel/sched/idtable.h"
#include "genos/panic.h"
#include "util/retype.h"
#include "util/math.h"
#include "string.h"

autom_schedee::autom_schedee(delegate<void> _dlg, int prio) : schedee()
{
	dlg = _dlg;
	sta_prio = prio;
	dyn_prio = prio;
}

void autom_schedee::execute() {
	dlg();
}

void autom_schedee::systick_handler() {
	panic("SYSTICKHANDLER");
}

void autom_schedee::signal_handler() {
	sig_t s = signals & signals_mask;
	if (bits_mask(s, SIGSTOP)) { bits_clr(signals, SIGSTOP); scheduler::set_held(this); return;}  
}

void automQuery(id_t id, const void* smsg, size_t slen, void* rmsg, size_t rlen) {
	dprln("automQuery");
	//virtee* target_vee = idtable::find(id);	
	RETYPE(struct autom_schedee*, asch, scheduler::get_current());

	asch->qry.smsg = smsg;
	asch->qry.rmsg = rmsg;
	asch->qry.slen = slen;
	asch->qry.rlen = rlen;
	asch->qry.sender = asch->pid;

//	target_vee->onquery(&asch->qry);		
	scheduler::set_block(asch, SCHEDEE_BLOCKED_QUERY);	
}

void automReceive(id_t id, void* msg, size_t len) {
	dprln("automReceive");
	RETYPE(struct autom_schedee*, asch, 
		scheduler::get_current());
	query* qry = asch->findQryForId(id); 
	if (qry == 0) {
		asch->qry.sender = id;
		asch->qry.smsg = msg;
		asch->qry.slen = len;
		scheduler::set_block(asch,SCHEDEE_BLOCKED_RECEIVE);
		return;
	}

	memcpy(msg, qry->smsg, min(len,qry->slen));

	//virtee* sender = idtable::find(qry->sender);
	//sender->onreceive();
}

void automReply(id_t id, const void* msg, size_t len) {
/*	RETYPE(struct autom_schedee*, asch, schedee_current_get());
	if (dlist_is_empty(&asch->schedee.qsocket)) {
		panic("empty socket on reply");
//		return -1;
	};

	struct query* qry =
	dlist_first_entry(&asch->schedee.qsocket, struct query, lnk);

	memcpy(qry->rmsg, msg, min(len,qry->rlen));

	struct virtee* sender = idtable_find(qry->sender);
	sender->ops->onreply(sender);*/
}
/*
struct schedee_operations autom_schedee_operations = {
	.execute = autom_schedee_execute,
	//.query = autom_query,
	//.receive = autom_receive,
	//.reply = autom_reply,
};

void autom_schedee_init(struct autom_schedee* asch, 
		void(*func)(void*), void* arg, int prio)
{
	asch->func = func;
	asch->arg = arg;
	schedee_init(&asch->schedee, &autom_schedee_operations, 0);
}

void automExit() {
	struct schedee* sch = schedee_current_get();
	schedee_set_zombie(sch);
	//RETYPE(struct autom_schedee*, asch, schedee_current_get());
	
}


/*
int8_t autom_receive(id_t id, void* msg, size_t len) {
	struct shedee* sender_sch = schedee_hashtable_find(id);	
	struct shedee* sch = schedee_current_get();
	RETYPE(autom_schedee*, asch, sch);
	
	if dlist_is_empty(&sch->qs.list) {
		sch->qs.waited_id = id;
		schedee_nonpreemptive_block(SCHEDEE_BLOCKED_RECEIVE);
		return 0;		
	};

	struct query* query;
	dlist_first_entry(query, struct query, lnk);

	memcpy(msg, query->smsg, max(len,query->slen));
	return 1;
}

int8_t autom_reply(id_t id, void* msg, size_t len) {
	struct shedee* sender_sch = schedee_hashtable_find(id);	
	struct shedee* sch = schedee_current_get();
	RETYPE(autom_schedee*, asch, sch);


}

*/