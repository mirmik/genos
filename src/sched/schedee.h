#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <stdint.h>
#include <stdbool.h>

#include <gxx/debug/dprint.h>
#include <gxx/datastruct/dlist.h>

#ifdef MVFS_INCLUDED
#	include <mvfs/mvfs.h>
#endif

#define SCHEDEE_STATE_RUN 		0
#define SCHEDEE_STATE_WAIT 		2
#define SCHEDEE_STATE_STOP 		3
#define SCHEDEE_STATE_FINAL 	4
#define SCHEDEE_STATE_ZOMBIE 	5

#define SCHEDEE_FDMAX 5

struct schedee_opertions;

struct schedee {
	struct schedee * parent;
	struct dlist_head lnk;
	uint8_t prio;
	uint8_t state;
	union {
		uint8_t flags;
		struct {
			uint8_t runned_flag : 1;
			uint8_t can_displace_flag : 1;
			uint8_t has_context_flag : 1;
		};
	};

#ifdef MVFS_INCLUDED
	struct node * pwd;
	struct file * fds [SCHEDEE_FDMAX]; //массив файловых дескрипторов
#endif

	const struct schedee_operations * sch_op;
};

struct schedee_operations {
	void (*execute) (struct schedee * sch);
	void (*displace) (struct schedee * sch);
	void (*finalize) (struct schedee * sch);
};

__BEGIN_DECLS

struct schedee * current_schedee();

static inline void schedee_init(struct schedee* sch, int prio, const struct schedee_operations * op) {
	dlist_init(&sch->lnk);
	sch->prio = prio;
	sch->state = SCHEDEE_STATE_STOP;
	sch->flags = 0;
	sch->sch_op = op;

	sch->parent = current_schedee(); 

#ifdef MVFS_INCLUDED
	sch->pwd = sch->parent ? sch->parent->pwd : vfs_get_root();
	for (int i = 0; i < SCHEDEE_FDMAX; ++i)
		sch->fds[i] = NULL;
#endif
} 

__END_DECLS

#endif