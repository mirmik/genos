#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <sys/cdefs.h>
#include <gxx/datastruct/dlist_head.h>

#include <genos/memory/heap.h>
#include <genos/memory/kmalloc.h>

#include <genos/time/stimer.h>

#define SCHEDEE_STATE_STOP 		0
#define SCHEDEE_STATE_RUN 		1
#define SCHEDEE_STATE_ZOMBIE 	2
#define SCHEDEE_STATE_FINAL 	3
#define SCHEDEE_STATE_WAIT 		4

struct schedee_s;
typedef void(*schops_execute_t)(struct schedee_s*);
typedef void(*schops_finalize_t)(struct schedee_s*);
typedef void(*schops_displace_t)(struct schedee_s*);

struct schedee_operations {
	schops_execute_t execute;
	schops_finalize_t finalize;
	schops_displace_t displace;
};

typedef struct schedee_s {
	struct dlist_head schlnk;
	const struct schedee_operations* ops;
	
	uint8_t state;
	uint8_t prio;

	uint16_t pid;
	stimer_t

	memory_heap_t* heap;
} schedee_t;

__BEGIN_DECLS

static inline void schedee_utilize(schedee_t* sch) { kfree(sch); }
static inline void schedee_execute(schedee_t* sch) { sch->ops->execute(sch); }
static inline void schedee_finalize(schedee_t* sch) { sch->ops->finalize(sch); }

static inline void schedee_init(schedee_t* sch, const struct schedee_operations* operations) {
	dlist_init(&sch->schlnk);
	sch->ops = operations;
	sch->state = SCHEDEE_STATE_STOP;
	sch->prio = 0;
	sch->heap = NULL;
	//sch->pid = get_new_pid();
}

//DEBUG: Имплементация schedee для тестирования shedee_manager
static inline void debug_schedee_execute(schedee_t* sch) {
	debug_print("debug_schedee_execute");
}

static const struct schedee_operations debug_schedee_operations = {
	.execute = &debug_schedee_execute
};

static inline void debug_schedee_init(schedee_t* sch) {
	schedee_init(sch, &debug_schedee_operations);
}

__END_DECLS

#endif