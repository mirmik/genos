#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

//#include <sys/cdefs.h>
#include "kernel/sched/schedee.h"

#define PRIORITY_TOTAL 5
#define ERRSCHEDBLOCK -2

namespace scheduler {

	void init();
	int schedule();
	void set_current(schedee*);
	schedee* get_current();

//	void registry(schedee& sch);
	void set_ready(schedee* sch, int8_t opt);
	void set_held(schedee* sch);
	void set_block(schedee* sch, uint16_t state);

	//void start(schedee& sch);
	//void stop(schedee& sch);
	
};

//void schedee_init(struct schedee* sch, struct schedee_operations* ops, int prio);
//void schedee_set_ready(struct schedee* sch, int8_t opt);
//void schedee_set_wait(struct schedee* sch, uint16_t state);
//void schedee_set_zombie(struct schedee* sch);

//uint8_t schedee_is_ready(struct schedee* sch);
//uint8_t schedee_is_blocked_query(struct schedee* sch);
//uint8_t schedee_is_blocked_receive(struct schedee* sch);
//uint8_t schedee_is_blocked_reply(struct schedee* sch);

//struct schedee* const schedee_current_get();
//void schedee_current_set(struct schedee* sch);

//void schedee_current_block(uint16_t state);
//void schedee_nonpreemptive_block(uint16_t state);

//uint8_t schedee_is_wait_msg(struct schedee* sch, id_t sender);
//void __schedee_query(struct schedee* tgt, id_t sender);
//void __schedee_target_receive(struct schedee* tgt);


//id_t getpid();
//__END_DECLS

#endif