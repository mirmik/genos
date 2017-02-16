#ifndef KERNEL_TASKLET_H
#define KERNEL_TASKLET_H

#include <datastruct/dlist_head.h>
#include <inttypes.h>

#define TASKLET_UNPLANED 0
#define TASKLET_PLANED 1


struct tasklet {
	struct dlist_head tlnk; //tasklet list
	uint8_t status;

	void(*action)(struct tasklet* tsk);
};


__BEGIN_DECLS

static inline void tasklet_init(struct tasklet* tsk, void(*action)(struct tasklet*)) {
	dlist_init(&tsk->tlnk);
	tsk->status = TASKLET_UNPLANED;
	tsk->action = action;
}

void tasklet_plan(struct tasklet* tsk);
void tasklet_unplan(struct tasklet* tsk);
void tasklet_execute(struct tasklet* tsk);

void tasklet_schedule();

__END_DECLS

#endif