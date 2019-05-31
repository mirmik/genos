#ifndef GENOS_RESMNGR_H
#define GENOS_RESMNGR_H

#include <igris/datastruct/dlist.h>
#include <genos/node.h>

namespace genos 
{
	class resmngr 
	{
	public:
		dlist_head lnk;
		const char * ns;

		resmngr(const char* ns) : ns(ns) {}

		virtual int lookup(const char* ns, genos::node** np) = 0;
		virtual int mkdir(const char* path) { return ENOTSUP; }
		virtual int rmdir(const char* path) { return ENOTSUP; }
	};
}

#endif