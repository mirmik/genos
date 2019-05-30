#include <genos/mvfs/vfsnode.h>
#include <genos/schedee.h>

int genos::open_node(genos::vfsnode * res) 
{
	int ans;
	int fd;

	ans = current_schedee()->restbl.get_available_fd(&fd);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	ans = res->open(&current_schedee()->restbl[fd]);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	return fd;
}