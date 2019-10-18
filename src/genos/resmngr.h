#ifndef GENOS_RESMNGR_H
#define GENOS_RESMNGR_H

#include <igris/datastruct/dlist.h>
#include <genos/resource.h>

//#error "That file will ever compiled?"

/**
	Ресурсный менеджер контролирует пространства имён
	ищет и открывает ресурс по его имени.
*/

namespace genos 
{
	extern struct dlist_head ns_list;

	class resmngr 
	{
	public:
		dlist_head lnk;
		const char * ns;

		resmngr(const char* ns) : ns(ns) 
		{
			dlist_add(&lnk, &ns_list);
		}

		virtual int open(const char* ns, genos::openres* np) = 0;
		virtual int mkdir(const char* path) { return ENOTSUP; }
		virtual int rmdir(const char* path) { return ENOTSUP; }
	};

	genos::resmngr * lookup_resmngr(const char* path, const char** epath);
	bool directory_exists(const char* path, size_t sz);
}

#endif