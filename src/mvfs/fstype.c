#include <mvfs/fstype.h>
#include <string.h>

DLIST_HEAD(mvfs_fslist);

struct file_system_type * mvfs_get_fs(const char* name) {
	struct file_system_type * it;
	dlist_for_each_entry(it, &mvfs_fslist, fs_list) {
		if (strncmp(name, it->name, FILESYSTEM_NAME_MAX) == 0) {
			return it;
		}
	}

	return NULL;
}

void mvfs_register_fs(struct file_system_type* fs) {
	dlist_add(&fs->fs_list, &mvfs_fslist);
}