#include <unistd.h>
#include <genos/api.h>
#include <genos/node.h>
#include <genos/schedee.h>

#include <igris/util/bug.h>

static 
genos::opennode * _get_schedee_file(int fd) 
{
	genos::schedee* sch = current_schedee();
	
	if (fd >= sch->restbl.tblsz)
		return nullptr;

	return &sch->restbl[fd];
}

ssize_t write(int fd, const void* data, size_t size) 
{
	genos::opennode * filp = _get_schedee_file(fd);
	if (filp == nullptr)
		return EBADF;

	if (!(filp->restype & GENOS_RESOURCE_FILE)) 
		return ENOTSUP; 

	return filp -> node -> write(data, size, filp->flags);
}

ssize_t read(int fd, void* data, size_t size) 
{
	BUG();
}

int readdir(int fd, char* buf, size_t maxsz) 
{
	genos::opennode * filp = _get_schedee_file(fd);
	if (filp == nullptr)
		return EBADF;

	if (!(filp->restype & GENOS_RESOURCE_DIRECTORY)) 
		return ENOTSUP; 

	return filp -> dir -> iterate(buf, maxsz, filp);
}