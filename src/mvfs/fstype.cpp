#include <mvfs/fstype.h>
#include <string.h>

#include <gxx/container/slist.h>

#include <gxx/debug/dprint.h>

gxx::slist<file_system_type, &file_system_type::fs_list> vfs_fslist;

struct file_system_type * vfs_get_fs(const char* name) 
{
	for ( auto& it : vfs_fslist ) {
		if (strncmp(name, it.name, FILESYSTEM_NAME_MAX) == 0) {
			return &it;
		}
	}
	
	return NULL;
}

void vfs_register_fs(struct file_system_type* fs) 
{
	vfs_fslist.add_first(*fs);
}