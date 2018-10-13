#include "schedee.h"
#include <mvfs/compat.h>

void shedee_mvfs_support_init(struct schedee* sch) {
	sch->pwd = sch->parent ? sch->parent->pwd : vfs_get_root();
	for (int i = 0; i < SCHEDEE_FDMAX; ++i)
		sch->fds[i] = NULL;
}