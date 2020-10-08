#include <genos/resource.h>
#include <genos/schedee.h>

#include <igris/util/errno.h>

int genos::open_resource(genos::resource * res, genos::openres * filp, int flags) 
{
	filp -> res = res;	
	filp -> flags = flags; // Надо понять, какие флаг вообще бывают.
	return res->open(filp);
}

int genos::open_resource(genos::resource * res, int16_t flags) 
{
	int ans;
	int fd;
	genos::openres * filp;

	fd = current_schedee()->restbl.get_available_fd();
	if (fd < 0) 
		return -1;

	filp = current_schedee()->restbl[fd];
	
	if ((ans = open_resource(res, filp, flags))) 
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

	filp = current_schedee()->restbl[fd];
	
	filp -> dir = res;
	
	(void) ans;
	ans = res->open(filp);
	if (ans) 
	{
		errno = ans;
		return -1;
	}

	return fd;
}

int genos::restbl::get_available_fd() 
{
	for (int i = 0; i < RESTBL_SIZE; ++i) 
	{
		if (tbl[i].node == nullptr) 
		{
			return i;
		}
	}

	return SET_ERRNO(-EMFILE);
}

int genos::node::print(const char * data, int flags)
{
	return write(data, strlen(data), flags);
}

int genos::node::println(const char * data, int flags)
{
	int ret = 0;
	ret += write(data, strlen(data), flags);
	ret += write("\r\n", 2, flags);
	return ret;
}