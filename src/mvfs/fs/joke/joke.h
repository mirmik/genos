#ifndef JOKE_FSTYPE_H
#define JOKE_FSTYPE_H

#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/dirent.h>

#include <gxx/panic.h>
#include <errno.h>

struct file_system_type joke_fstype;
const struct node_operations joke_i_op;
const struct file_operations joke_f_op;

static struct node fixino;

struct joke_super_block {
	struct super_block sb;
} joke_sb;

static inline struct super_block * joke_get_sb(struct file_system_type *fs, int flags, const void * data) {
	static uint8_t inited = 0;

	if (inited == 0) {
		inited = 1;
		
		joke_sb.sb.s_fs = &joke_fstype;
		joke_sb.sb.i_op = &joke_i_op;
		joke_sb.sb.f_op = &joke_f_op;
		joke_sb.sb.s_root = &fixino;

		fixino.i_sb = &joke_sb.sb;
		fixino.directory_flag = 1;
		fixino.nlink++;
	}

	return &joke_sb.sb;
}

static void joke_kill_sb(struct super_block * sb) {
	panic("joke sb deallocated");
}

static int joke_create_inode(struct inode * parent, struct dentry * carrier, uint8_t mode) {
	fixino.nlink++;
	carrier->d_inode = &fixino;
	return 0;
}

static int joke_put_inode(struct inode * i) {
	(void) i;
	return 0;
}

static int joke_mkdir(struct inode * i, struct dentry * d, int m) {
	d->d_inode = &fixino;
	return 0;
}

static int joke_rmdir(struct inode * i, struct dentry * d) {
	d->d_inode = NULL;
	return 0;
}

static int joke_iterate(struct file * filp, void * priv) {
	struct dirent * de = (struct dirent *) priv;
	struct dentry * parent;
	struct dentry * d;
	struct dlist_head * it;

	if (filp->f_inode->directory_flag == 0) 
		return ENOTSUP;

	parent = filp->f_dentry;

	it = parent->childrens.next;//dlist_entry_first(&parent->childrens, struct dentry, lnk);

	int n = filp->pos;
	while(n--) it = it->next;
	filp->pos++;

	if (it == &parent->childrens) de->d_name[0] = '\0';
	else {
		d = dlist_entry(it, struct dentry, lnk);
		de->d_ino = 0;
		strcpy(de->d_name, d->name);
	}
	return 0;
}

struct file_system_type joke_fstype = {
	.name = "joke",
	.get_sb = joke_get_sb,
	.kill_sb = joke_kill_sb
};

const struct inode_operations joke_i_op = {
	//.create = joke_create_inode,
	.lookup = vfs_virtual_lookup,
	.mkdir = joke_mkdir,
	.rmdir = joke_rmdir,
};

const struct file_operations joke_f_op = {
	.open = vfs_common_open,
	.release = vfs_common_release,
	.write = NULL,
	.read = NULL,
	.iterate = joke_iterate,
};

#endif