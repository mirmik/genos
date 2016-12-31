#include "kernel/sched/schedee.h"

#include "kernel/sched/sched.h"
#include "gxx/algorithm.h"
#include "debug/dprint.h"
#include "util/bits.h"

schedee::schedee() {
	type = VIRTTYPE_SCHEDEE;
	bits_clr_all(signals);
	bits_set_all(signals_mask); 
	//scheduler::registry(*this);
}

void schedee::start() {
	scheduler::set_ready(this, 0);
}

void schedee::stop() {
	//scheduler::set_held(*this);
	signal::send(this, SIGSTOP);
}

void schedee::onquery(query* qry){
	if (is_blocked_receive()) {
		dprln("blocked receive");
	}
	else
		qsocket.push_back(*qry);
}

void schedee::onreceive(){

}

void schedee::onreply(){

}

query* schedee::findQryForId(id_t id) {
	auto begin = qsocket.begin();
	auto end = qsocket.end();
	dlist<query,&query::lnk>::iterator it = qsocket.begin();

	if (id != 0) it = gxx::find_if(begin, end, [id](query& qry) -> bool {
			return qry.sender == id;
	});

	if (it == end) return 0;
	return &*it;
};