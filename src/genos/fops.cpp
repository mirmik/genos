#include <unistd.h>
#include <genos/schedee.h>

ssize_t write(int fd, const void* data, size_t size) 
{
	int ans;
	genos::opened_resource * ores;

	schedee* sch = current_schedee();
	
	ans = sch->restbl.getfd(fd, &ores);
	if (ans)
		return ans;

	if (ores->res->resource_type & RESOURCE_STREAM)
		return ((genos::stream_resource*)(ores->res))->write(data, size, ores->flags);
	else 
	{
		errno = ENOTSUP;
		return -1;
	}
}

ssize_t read(int fd, void* data, size_t size) 
{
	
}