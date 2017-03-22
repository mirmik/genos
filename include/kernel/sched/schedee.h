#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <kernel/service/service.h>

#define SCHEDULE_RESUME 1
#define SCHEDULE_REPEAT 0

static const uint8_t SCHEDEE_STATE_INIT = 0x00;
static const uint8_t SCHEDEE_STATE_RUN = 0x01;
static const uint8_t SCHEDEE_STATE_WAIT = 0x02;
static const uint8_t SCHEDEE_BLOCKED_SEND = 0x03;
static const uint8_t SCHEDEE_BLOCKED_RECEIVE = 0x04;
static const uint8_t SCHEDEE_STATE_FINAL = 0x05;
static const uint8_t SCHEDEE_STATE_ZOMBIE = 0x06;
static const uint8_t SCHEDEE_BLOCKED_STOP = 0x07;

static const uint8_t SCHEDEE_STATE_MASK = 0x0F;

static const uint8_t SCHEDEE_FINAL_DEALLOCATE = 0x80;
static const uint8_t SCHEDEE_FINAL_RELEASE = 0x40;

/*struct schedee_operations {
	uint8_t (*execute)	(struct schedee*);
	uint8_t (*engage)	(struct schedee*);
	uint8_t (*displace)	(struct schedee*);
	uint8_t (*lastexit)	(struct schedee*);
	uint8_t (*destructor)(struct schedee*);
};*/

/*struct schedee {
	struct service srvs;

	struct dlist_head lnk;
	uint8_t prio;
	uint8_t flags;

	const struct schedee_operations* schops;
};*/

namespace genos {
	class schedee : public service {
	public:
		struct dlist_head schlnk;
		uint8_t prio;
		uint8_t flags;

	public:
		schedee();

		void run();
		void stop();
		void exit();

		void set_state_wait(uint8_t state);
		void set_state_run();
		void set_state_final();
		void set_state_zombie();
		
		bool state_is(uint8_t state);
		
		void set_final_deallocate(bool en);
		void set_final_release(bool en);
		
		bool is_running();
		bool is_waiting();
		bool is_zombie();
		bool is_final_deallocated();
		bool is_final_release();
	};

	void schedee_exit();

	struct schedee* current_schedee();
	void set_current_schedee(struct schedee* sch);

	void finalize_schedee(struct schedee* sch);
}

/*__BEGIN_DECLS

void schedee_init(struct schedee* sch, 
	const struct schedee_operations* schops, 
	const struct service_operations* srvsops);

void schedee_run(struct schedee* sch);
void schedee_stop(struct schedee* sch);

void schedee_exit();

struct schedee* current_schedee();
void set_current_schedee(struct schedee* sch);

__END_DECLS*/

#endif