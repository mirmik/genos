#include <genos/node.h>
#include <genos/schedee.h>

int genos::open_node(genos::node * res) 
{
	int ans;
	int fd;
	genos::opennode * filp;

	ans = current_schedee()->restbl.get_available_fd(&fd);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	filp = & current_schedee()->restbl[fd];
	filp -> node = res;	
	filp -> restype = GENOS_RESOURCE_FILE;

	ans = res->open(filp);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	return fd;
}

int genos::open_directory(genos::directory * res) 
{
	int ans;
	int fd;
	genos::opennode * filp;

	ans = current_schedee()->restbl.get_available_fd(&fd);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	filp = & current_schedee()->restbl[fd];
	
	filp -> dir = res;
	filp -> restype = GENOS_RESOURCE_DIRECTORY;

	/*ans = res->open(filp);
	if (ans) 
	{
		errno = ans;
		return -1;
	}*/

	return fd;
}