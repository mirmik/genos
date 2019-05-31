#ifndef GENOS_DEV_H
#define GENOS_DEV_H

#include <genos/node.h>
#include <genos/resmngr.h>

namespace genos 
{
	class device_manager : public genos::resmngr, public genos::named_node_list
	{
	public:
		device_manager() : genos::resmngr("dev") {}

		int lookup(const char* ns, genos::node** np) override
		{
			dlist_head* it;
			dlist_for_each(it, &list) 
			{
				genos::node * nit = dlist_entry(it, genos::node, lnk);
				if (strcmp(nit->name, ns) == 0)
				{
					*np = nit;
					return 0;
				}
			}
			return ENOENT;
		}
	};

	extern device_manager devmngr;
}

#endif