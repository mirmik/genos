#include <genos/resmngr.h>

struct dlist_head genos::ns_list = DLIST_HEAD_INIT(genos::ns_list);

genos::resmngr * genos::lookup_resmngr(const char* path, const char** epath) 
{
	int maxlen = 0;
	genos::resmngr * maxmngr = nullptr;

	const char* cptr;
	genos::resmngr * it;

	dlist_for_each_entry(it, &genos::ns_list, lnk) 
	{
		int len;
		const char * ns = it->ns;
		cptr = path;

		while(*ns == *cptr && *ns) 
		{
			++ns;
			++cptr;
		}
		len = cptr - path;

		if (len > maxlen) 
		{
			maxlen = len;
			maxmngr = it; 
		}
	}

	*epath = path + maxlen;
	return maxmngr;
}