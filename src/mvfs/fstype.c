#include <mvfs/fstype.h>
#include <string.h>

SLIST_HEAD(vfs_fslist);

struct file_system_type * vfs_get_fs(const char* name) 
{
	struct file_system_type * it;

	slist_for_each_entry(it, &vfs_fslist, fs_list) {
		if (strncmp(name, it->name, FILESYSTEM_NAME_MAX) == 0) {
			return it;
		}
	}

	return NULL;
}

void vfs_register_fs(struct file_system_type* fs) 
{
	slist_add(&fs->fs_list, &vfs_fslist);
}