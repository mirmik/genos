#include <mvfs/fstype.h>
#include <string.h>

#include <igris/datastruct/slist.h>

//igris::slist<file_system_type, &file_system_type::fs_list> vfs_fslist;

SLIST_HEAD(vfs_fslist);

struct file_system_type * vfs_get_fs(const char* name) 
{
	struct file_system_type * it;
	slist_for_each_entry ( it, &vfs_fslist, lnk ) {
		if (strncmp(name, it->name, FILESYSTEM_NAME_MAX) == 0) {
			return it;
		}
	}
	
	return NULL;
}

void vfs_register_fs(struct file_system_type* fs) 
{
	slist_add(&fs->lnk, &vfs_fslist);
}