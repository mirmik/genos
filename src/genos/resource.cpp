#include <genos/resource.h>
#include <genos/schedee.h>

int genos::open_node(genos::node * res, genos::openres * filp) 
{
	filp -> node = res;	
	filp -> restype = GENOS_RESOURCE_FILE;

	return res->open(filp);
}

int genos::open_node(genos::node * res, int flags) 
{
	int ans;
	int fd;
	genos::openres * filp;

	fd = current_schedee()->restbl.get_available_fd();
	if (fd < 0) 
		return -1;

	filp = & current_schedee()->restbl[fd];
	
	if ((ans = open_node(res, filp))) 
		return ans;

	filp -> flags = flags;

	return fd;
}


int genos::open_directory(genos::directory * res) 
{
	int ans;
	int fd;
	genos::openres * filp;

	fd = current_schedee()->restbl.get_available_fd();
	if (fd < 0) 
		return -1;

	filp = & current_schedee()->restbl[fd];
	
	filp -> dir = res;
	filp -> restype = GENOS_RESOURCE_DIRECTORY;

	(void) ans;
	/*ans = res->open(filp);
	if (ans) 
	{
		errno = ans;
		return -1;
	}*/

	return fd;
}

int genos::restbl::get_available_fd() 
{
	for (int i = 0; i < tblsz; ++i) 
	{
		if (tbl[i].node == nullptr) 
		{
			return i;
		}
	}

	return SET_ERRNO(-EMFILE);
}