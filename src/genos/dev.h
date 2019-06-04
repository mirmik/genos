#ifndef GENOS_DEV_H
#define GENOS_DEV_H

#include <genos/resource.h>
#include <genos/resmngr.h>

namespace genos 
{
	class device_manager : public genos::resmngr, public genos::named_node_list
	{
	public:
		device_manager() : genos::resmngr("/dev"), genos::named_node_list() {}

		int open(const char* ns, genos::openres* filp) override
		{
			dlist_head* it;
			ns++; // Drop first '/'

			dlist_for_each(it, &list) 
			{
				genos::named_node * nit = dlist_entry(it, genos::named_node, lnk);
				if (strcmp(nit->name, ns) == 0)
				{
					return open_resource(nit, filp);
				}
			}
			return SET_ERRNO(-ENOENT);
		}

		void link(genos::named_node* node) 
		{
			dlist_add(&node->lnk, &list);
		}
	};

	extern device_manager devmngr;
}

#endif