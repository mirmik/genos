#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <kernel/sched/schedee.h>

class automate_schedee : public schedee {
protected:
	automate_schedee();
	~automate_schedee() { }

protected:
	void execute() override {
		Routine();
	}

	virtual void invalidate() override {}
	virtual void Routine() = 0;
};

int autom_schedee_send_query(struct service* s, struct query *q) {
	struct automate_schedee *asch = (automate_schedee*) s;
	debug_print("send_query");
	return ERROR_REPLY;
}

int autom_schedee_receive_query(struct service* s, struct query *q) {
	struct automate_schedee *asch = (automate_schedee*) s;
	debug_print("add_query");
	return ERROR_REPLY;
}

int autom_schedee_reply_answer(struct service* s, struct query *q) {
	struct automate_schedee *asch = (automate_schedee*) s;
	debug_print("send_query");
	return ERROR_REPLY;
}

int autom_schedee_receive_answer(struct service* s, struct query *q) {
	struct automate_schedee *asch = (automate_schedee*) s;
	debug_print("send_query");
	return ERROR_REPLY;
}

static const struct service_operations autom_schedee_service_operations = {
	.send_query = autom_schedee_send_query,
	.receive_query = autom_schedee_receive_query,
	.reply_answer = autom_schedee_reply_answer,
	.receive_answer = autom_schedee_receive_answer,
};

automate_schedee::automate_schedee() { 
	hops = &autom_schedee_service_operations;
}

#endif