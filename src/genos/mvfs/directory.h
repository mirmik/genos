#ifndef GENOS_MVFS_DIRECTORY_H
#define GENOS_MVFS_DIRECTORY_H

#include <genos/mvfs/vfsnode.h>

namespace genos 
{
	class directory : public vfsnode
	{
	public:
		void add_child(vfsnode * child) 
		{
			dlist_add_tail(&child->lnk, &childs);
		}

		directory(const char* name) : vfsnode(name, 0) {}
	};

	extern directory root_directory;
}

#endif