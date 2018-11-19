#include <sched/schedee.h>
#include <sched/sched.h>
#include <errno.h>

#ifdef MVFS_INCLUDED

int schedee_get_free_fd(struct schedee * sch) {
	for(int i = 0; i < SCHEDEE_FDMAX; ++i) {
		if (sch->fds[i] == NULL)
			return i;
	}
	return -1;
}

int schedee_setfd(struct schedee * sch, struct file * filp, int fd) {
	if (sch->fds[fd] != NULL) 
		return ENOENT;

	sch->fds[fd] = filp;
	return 0;
}

#endif //MVFS_INCLUDED

void schedee_notify_finalize(struct schedee * sch) {
	if (sch->parent != NULL && sch->parent->state == SCHEDEE_STATE_WAIT_SCHEDEE) {
		schedee_run(sch->parent); 
	}
}