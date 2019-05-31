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

		int lookup(const char* ns, genos::node** np) 
		{
		/*	genos::named_node* nit;
			dlist_for_each_entry(nit, &list, lnk) 
			{
				if (strcmp(nit->name, ns) == 0)
				{
					*np = nit;
					return 0;
				}
			}
			return ENOENT;*/
		}
	};

	extern device_manager devmngr;
}

#endif