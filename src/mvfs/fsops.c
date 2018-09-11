#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/dentry.h>
#include <mvfs/variant/global_root.h>

#include <gxx/panic.h>
#include <gxx/debug/dprint.h>

#include <errno.h>

int mvfs_mount_first(const char* fstype, unsigned long mountflags, const void *data) {
	struct file_system_type * fs = mvfs_get_fs(fstype);

	struct super_block * sb = fs->get_sb(fs, mountflags, data);
	mvfs_set_global_root(sb->s_root);

	return 0;
}

int mvfs_mount(const char *source, const char *target, 
	const char* fstype, unsigned long mountflags, const void *data
) {
	panic("TODO");
}

int mvfs_mkdir(const char *path) {
	debug_print_line("mvfs_mkdir");
}

int mvfs_chdir(const char *path) {
	struct dentry* tgt;
	int sts;

	if (sts = mvfs_lookup(path, &path, &tgt)) {
		return sts;
	}

	///Убеждаемся, что дентри - директория. 
	if (dlist_empty(&tgt->d_tree.children)) {
		///Если у дентри нет потомков, проверяем, что думает и-нода.
		if (!tgt->d_inode || !tgt->d_inode->directory_flag) {
			return -ENOTDIR;
		}
	}

	mvfs_set_pwd(tgt);
	return 0;
}

int mvfs_chroot(const char *path) {
	struct dentry* tgt;
	int sts;

	if (sts = mvfs_lookup(path, &path, &tgt)) {
		return sts;
	}

	mvfs_set_root(tgt);
	return 0;
}