#include <kernel/tasks/tasklet.h>
#include <kernel/csection.h>

static struct dlist_head tasks_queue = DLIST_HEAD_INIT(tasks_queue);
static struct dlist_head hi_tasks_queue = DLIST_HEAD_INIT(hi_tasks_queue);

void tasklet_plan(struct tasklet* tsk) {
	atomic_section_enter();

	dlist_add(&tsk->tlnk, &tasks_queue);
	tsk->status = TASKLET_PLANED;
	
	atomic_section_leave();
}

void tasklet_hi_plan(struct tasklet* tsk) {
	atomic_section_enter();

	dlist_add(&tsk->tlnk, &hi_tasks_queue);
	tsk->status = TASKLET_PLANED;
	
	atomic_section_leave();
}

void tasklet_unplan(struct tasklet* tsk) { 
	atomic_section_enter();

	dlist_del(&tsk->tlnk);
	tsk->status = TASKLET_UNPLANED;
	
	atomic_section_leave();;
}

#define tasklet_hi_unplan(tsk) tasklet_unplan(tsk)

void tasklet_schedule() {
	__label__ start;
	struct tasklet* tsk;

	start:

	if (!dlist_empty(&hi_tasks_queue)) {
		tsk = dlist_first_entry(&hi_tasks_queue, struct tasklet, tlnk);
		dlist_del(&tsk->tlnk);
		tsk->status = TASKLET_UNPLANED;
		tsk->action(tsk);
		goto start;
	}

	if (!dlist_empty(&tasks_queue)) {
		tsk = dlist_first_entry(&tasks_queue, struct tasklet, tlnk);
		dlist_del(&tsk->tlnk);
		tsk->status = TASKLET_UNPLANED;
		tsk->action(tsk);
		goto start;
	}
}