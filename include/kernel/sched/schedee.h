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

struct schedee;

struct schedee_operations {
	uint8_t (*execute)	(struct schedee*);
	uint8_t (*engage)	(struct schedee*);
	uint8_t (*displace)	(struct schedee*);
	uint8_t (*lastexit)	(struct schedee*);
	uint8_t (*destructor)(struct schedee*);
};

struct schedee {
	struct service srvs;

	struct dlist_head lnk;
	uint8_t prio;
	uint8_t flags;

	const struct schedee_operations* schops;
};

__BEGIN_DECLS

void schedee_init(struct schedee* sch, 
	const struct schedee_operations* schops, 
	const struct service_operations* srvsops);

void schedee_run(struct schedee* sch);
void schedee_stop(struct schedee* sch);

void schedee_exit();

struct schedee* current_schedee();
void set_current_schedee(struct schedee* sch);

__END_DECLS

#endif