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


uint8_t autom_schedee_receive_query(struct service* s, struct query *q) {
	struct automate_schedee *asch = (automate_schedee*) s;
	debug_print("add_query");
	return ERROR_REPLY;
}

static const struct service_operations autom_schedee_service_operations = {
	.receive_query = autom_schedee_receive_query,
	do_nothing
};

automate_schedee::automate_schedee() { 
	hops = &autom_schedee_service_operations;
}