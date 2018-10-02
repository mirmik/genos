#include <sched/schedee.h>

int schedee_get_free_fd(struct schedee * sch) {
	for(int i = 0; i < SCHEDEE_FDMAX; ++i) {
		if (sch->fds[i] == NULL)
			return i;
	}
	return -1;
}