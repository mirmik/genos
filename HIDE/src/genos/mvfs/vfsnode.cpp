#include <genos/mvfs/vfsnode.h>
#include <genos/schedee.h>

int genos::open_node(genos::vfsnode * res) 
{
	int ans;
	int fd;
	genos::file * filp;

	ans = current_schedee()->restbl.get_available_fd(&fd);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	filp = & current_schedee()->restbl[fd];
	filp -> node = res;	

	ans = res->open(filp);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	return fd;
}