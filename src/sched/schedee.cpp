#include "schedee.h"

#ifdef MVFS_INCLUDED
#	include <mvfs/mvfs.h>
#endif

void shedee_mvfs_support_init(struct schedee* sch) {
	sch->pwd = sch->parent ? sch->parent->pwd : vfs_get_root();
	for (int i = 0; i < SCHEDEE_FDMAX; ++i)
		sch->fds[i] = NULL;
}