#include <unistd.h>
#include <genos/schedee.h>

#include <igris/util/bug.h>

ssize_t write(int fd, const void* data, size_t size) 
{
	int ans;
	genos::opennode * filp;

	schedee* sch = current_schedee();
	
	ans = sch->restbl.get_file(fd, &filp);
	if (ans)
		return ans;

	return filp -> node -> write(data, size, filp->flags);
}

ssize_t read(int fd, void* data, size_t size) 
{
	BUG();
}