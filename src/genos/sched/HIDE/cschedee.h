#ifndef GENOS_CSHEDEE_H
#define GENOS_CSHEDEE_H

#include <stdint.h>
#include <util/retype.h>
#include <util/stub.h>

const uint8_t SCHEDEE_STATE_RUN = 1;
const uint8_t SCHEDEE_STATE_STOP = 2;
const uint8_t SCHEDEE_STATE_KILL = 4;
const uint8_t SCHEDEE_STATE_MASK = SCHEDEE_STATE_RUN | SCHEDEE_STATE_STOP | SCHEDEE_STATE_KILL;

struct schedee_t;

struct schedee_operations {
	void(*execute)(struct schedee_t*);
	void(*destruct)(struct schedee_t*);
};

struct schedee_t {
	dlist_head lnk;

	uint16_t static_prio;
	uint16_t prio;
	uint8_t flags;

	const struct schedee_operations* ops;
};

struct func_schedee_t {
	schedee_t sch;
	void(*func)();
};

typedef struct schedee_t schedee_t; 

static inline void schedee_init(struct schedee_t * sch, int8_t prio = 0);
static inline void schedee_init(struct schedee_t * sch, int8_t prio) {
	dlist_init(&sch->lnk);
	sch->static_prio = prio;
	sch->prio = prio;
	sch->flags = 0;
} 

static inline void schedee_setPriority(struct schedee_t * sch, int8_t prio) {
	sch->static_prio = prio;
	sch->prio = prio;
} 

static inline void func_schedee_execute(struct schedee_t * sch) {
	RETYPE(struct func_schedee_t*, fsch, sch);
	fsch->func();
}

static const schedee_operations func_schedee_operations = {
	.execute = func_schedee_execute,
	.destruct = do_nothing
};

static inline void func_schedee_init(struct func_schedee_t * fsch, void(*func)(), int8_t prio = 0);
static inline void func_schedee_init(struct func_schedee_t * fsch, void(*func)(), int8_t prio) {
	schedee_init(&fsch->sch, prio);
	fsch->func = func;
	fsch->sch.ops = &func_schedee_operations;
} 

#endif