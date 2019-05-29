#include <genos/resource.h>

#include <genos/schedee.h>

int genos::open_resource(resource* res) 
{
	int ans;
	int fd;

	ans = current_schedee()->restbl.get_available_resource(&fd);
	if (ans) 
		return ans;

	ans = res->open(&current_schedee()->restbl[fd]);

	return fd;
}

int genos::resource_table::get_available_resource(int * fdp) 
{
	for (int i = 0; i < tblsz; ++i) 
	{
		if (tbl[i].res == nullptr) 
		{
			*fdp = i;
			return 0;
		}
	}

	return -1;
}