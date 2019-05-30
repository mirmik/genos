#ifndef GENOS_MVFS_SUPER_H
#define GENOS_MVFS_SUPER_H

#include <genos/mvfs/vfsnode.h>
#include <genos/mvfs/fstype.h>

namespace genos 
{
	class super_block 
	{
		struct dlist_head sblocks; 
	
		const struct file_system_type * fs;
		struct vfsnode * root; ///корневой dentry ветви.
		uint16_t 		refs;

		vfsnode * mount_point;
	};
}

#endif