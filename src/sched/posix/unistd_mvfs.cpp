#ifdef MVFS_INCLUDED

#include <sched/sched.h>
#include <mvfs/mvfs.h>
#include <mvfs/file.h>

#include <unistd.h>
#include <errno.h>

static inline int get_filp(int fd, struct file ** filpp) {
	struct schedee * sch = current_schedee();
		if (sch == NULL) 
			return ENOTSUP;

	*filpp = sch->fds[fd];

	return 0;
}

ssize_t write(int fd, const void *buf, size_t nbyte) 
{
	struct file * filp;

	int sts = get_filp(fd, &filp);
		if (sts) 
			return sts;

	return vfs_write(filp, (const char*)buf, nbyte);
}

ssize_t read(int fd, void *buf, size_t nbyte) 
{
	struct file * filp;

	int sts = get_filp(fd, &filp);
		if (sts) 
			return sts;

	return vfs_read(filp, (char*)buf, nbyte);
}

int dup2(int oldfd, int newfd) 
{
	struct file * filp;
	
	struct schedee * sch = current_schedee();
		if (sch == NULL) 
			return ENOTSUP;
	
	filp = sch->fds[oldfd];
	filp->refs++;
	sch->fds[newfd] = filp;

	return 0;
}

int dup(int oldfd) 
{
	struct schedee * sch = current_schedee();
		if (sch == NULL) 
			return ENOTSUP;

	int minfd = schedee_get_free_fd(sch);
	return dup2(oldfd, minfd);
}


#endif