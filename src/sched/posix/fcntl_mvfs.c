#ifdef MVFS_INCLUDED

#include <sched/sched.h>
#include <mvfs/mvfs.h>

#include <fcntl.h>
#include <errno.h>

int open(const char *path, int __oflag, ...) 
{
	struct file * filp;
	struct schedee * sch;
	int sts;
	int fdno;

	sch = current_schedee();
		if (sch == NULL) 
			return ENOTSUP;

	sts = vfs_open(path, __oflag, &filp);
		if (sts)
			return sts;

	fdno = schedee_get_free_fd(sch);
		if (fdno < 0)
			//TODO
			return ENOTSUP;

	sch->fds[fdno] = filp;

	return fdno;
}

#endif