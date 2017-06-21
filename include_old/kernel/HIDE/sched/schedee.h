#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include "inttypes.h"
#include "gxx/container/dlist.h"
#include "genos/datastruct/hlist_head.h"
//#include "kernel/sched/pid.h"
#include "kernel/sched/virtee.h"
#include "kernel/sched/query.h"
#include "util/bits.h"

#include "kernel/signals.h"

#define SCHEDEE_READY 			0x0001
#define SCHEDEE_HELD			0x0002

#define SCHEDEE_BLOCKED_QUERY 	0x0004
#define SCHEDEE_BLOCKED_RECEIVE	0x0008
#define SCHEDEE_BLOCKED_REPLY	0x0010
#define SCHEDEE_ZOMBIE			0x0020

#define SCHEDEE_STATE_MASK		0x003F
#define SCHEDEE_STATE_WAIT_MASK	0x001E

//Флаг указывает, что передачу управления диспетчеру при
//вызове блокирующих операций процесс берёт на себя.
//Кроме того, он не может вытесняться.
#define SCHEDEE_NONPREEMPTIBLE	0x1000

#define VIRTTYPE_SCHEDEE 1

class schedee : public virtee
{	
public:
	struct dlist_head statelnk;
	sig_t signals;
	sig_t signals_mask;

	dlist<query, &query::lnk> qsocket;
	
	uint16_t flags;
	int8_t dyn_prio;
	int8_t sta_prio;

public:

	inline bool is_ready() {return bits_mask(flags,SCHEDEE_READY);};
	inline bool is_blocked_receive() {
		return bits_mask(flags,SCHEDEE_BLOCKED_RECEIVE);
	}

	virtual void execute() = 0;
	virtual void systick_handler() = 0;	 
	virtual void signal_handler() = 0;
	
	//virtee methods
	void onquery(query* qry);
	void onreceive();
	void onreply();

	query* findQryForId(id_t id);

	schedee();
	void start();
	void stop();
};

#endif