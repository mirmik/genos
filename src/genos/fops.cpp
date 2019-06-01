#include <unistd.h>
#include <fcntl.h>

#include <genos/api.h>

#include <genos/resource.h>
#include <genos/resmngr.h>
#include <genos/schedee.h>

#include <igris/util/bug.h>

static 
genos::openres * _get_schedee_file(int fd) 
{
	genos::schedee* sch = current_schedee();
	
	if (fd >= sch->restbl.tblsz)
	{
		errno = EBADF;
		return nullptr;
	}

	return &sch->restbl[fd];
}

ssize_t write(int fd, const void* data, size_t size) 
{
	genos::openres * filp = _get_schedee_file(fd);
	if (filp == nullptr)
		return -1;

	if (!(filp->restype & GENOS_RESOURCE_FILE)) 
	{
		errno = ENOTSUP;
		return -1; 
	}

	return filp -> node -> write(data, size, filp->flags);
}

ssize_t read(int fd, void* data, size_t size) 
{
	genos::openres * filp = _get_schedee_file(fd);
	if (filp == nullptr)
		return SET_ERRNO(-EBADF);

	if (!(filp->restype & GENOS_RESOURCE_FILE)) 
	{
		return SET_ERRNO(-ENOTSUP); 
	}

	return filp -> node -> read(data, size, filp->flags);
}

int readdir(int fd, char* buf, size_t maxsz) 
{
	genos::openres * filp = _get_schedee_file(fd);
	if (filp == nullptr)
		return -1; 
	
	if (!(filp->restype & GENOS_RESOURCE_DIRECTORY)) 
	{
		errno = ENOTSUP;
		return -1; 
	}

	return filp -> dir -> iterate(buf, maxsz, filp);
}

int open(const char * path, int flags, ...) 
{
	int fd;
	int ans;
	genos::openres * filp;
	genos::schedee* sch; 
	const char * epath;

	sch = current_schedee();

	fd = sch->restbl.get_available_fd();
	if (fd < 0)
		return -1;

	filp = _get_schedee_file(fd);
	if (filp == nullptr) 
		return -1;

	genos::resmngr * resmngr = genos::lookup_resmngr(path, &epath);
	if (resmngr == NULL)
		return SET_ERRNO(-ENOSYS);

	if ((ans = resmngr->open(epath, filp)))
		return ans;

	filp -> flags = flags;

	return fd;
}