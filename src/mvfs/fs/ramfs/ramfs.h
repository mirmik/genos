#ifndef RAMFS_FSTYPE_H
#define RAMFS_FSTYPE_H

#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>

#include <gxx/panic.h>
#include <errno.h>

struct file_system_type ramfs_fstype;
const struct inode_operations ramfs_i_op;

struct ramfs_super_block {
	struct super_block sb;
} ramfs_sb;

static inline struct super_block * ramfs_get_sb(struct file_system_type *fs, int flags, const void * data) {
	static uint8_t inited = 0;

	if (inited == 0) {
		inited = 1;
		ramfs_sb.sb.s_fs = &ramfs_fstype;
		ramfs_sb.sb.i_op = &ramfs_i_op;
		ramfs_sb.sb.s_root = vfs_dentry_create("/");
		ramfs_sb.sb.s_root->d_inode = NULL;
	}

	return &ramfs_sb.sb;
}

static inline void ramfs_kill_sb(struct super_block * sb) {
	panic("ramfs sb deallocated");
}

static inline struct inode * ramfs_get_inode() {
	return &ramfs_fix_inode;
}

static inline int ramfs_put_inode(struct inode * i) {
	(void) i;
	return 0;
}

struct file_system_type ramfs_fstype = {
	.name = "ramfs",
	.get_sb = ramfs_get_sb,
	.kill_sb = ramfs_kill_sb
};

const struct inode_operations ramfs_i_op = {
	.alloc_inode = ramfs_get_inode,
	.destroy_inode = ramfs_put_inode,
	.lookup = vfs_virtual_lookup,
};

#endif