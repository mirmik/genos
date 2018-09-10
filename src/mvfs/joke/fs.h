#ifndef JOKE_FSTYPE_H
#define JOKE_FSTYPE_H

#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/dentry.h>

#include <gxx/panic.h>
#include <errno.h>

struct file_system_type joke_fstype;
const struct inode_operations joke_i_op;

struct inode joke_fix_inode;

struct joke_super_block {
	struct super_block sb;
} joke_sb;

static inline struct super_block * joke_get_sb(struct file_system_type *fs, int flags, const void * data) {
	static uint8_t inited = 0;

	if (inited == 0) {
		inited = 1;
		joke_sb.sb.s_fs = &joke_fstype;
		joke_sb.sb.i_op = &joke_i_op;
		joke_sb.sb.s_root = mvfs_dentry_create("/");
		joke_sb.sb.s_root->d_inode = NULL;
	}

	return &joke_sb.sb;
}

static inline void joke_kill_sb(struct super_block * sb) {
	panic("joke sb deallocated");
}

static inline struct inode * joke_get_inode() {
	return &joke_fix_inode;
}

static inline int joke_put_inode(struct inode * i) {
	(void) i;
	return 0;
}

struct file_system_type joke_fstype = {
	.name = "joke",
	.get_sb = joke_get_sb,
	.kill_sb = joke_kill_sb
};

const struct inode_operations joke_i_op = {
	.alloc_inode = joke_get_inode,
	.destroy_inode = joke_put_inode,
	.lookup = mvfs_virtual_lookup,
};

#endif