#include <genos/mvfs/fdtable.h>

int genos::fdtable::get_available_fd(int * fdp) 
{
	for (int i = 0; i < tblsz; ++i) 
	{
		if (tbl[i].node == nullptr) 
		{
			*fdp = i;
			return 0;
		}
	}

	return -1;
}