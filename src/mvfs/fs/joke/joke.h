#ifndef JOKE_FSTYPE_H
#define JOKE_FSTYPE_H

#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/dirent.h>

#include <gxx/panic.h>
#include <errno.h>
#include <string.h>

#include <gxx/debug/dprint.h>

struct file_system_type joke_fstype;
const struct node_operations joke_i_op;
const struct file_operations joke_f_op;

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
		joke_sb.sb.s_root = virtual_node_create("/", 1);
		joke_sb.sb.s_root->directory_flag = 1;
		joke_sb.sb.s_root->i_sb = &joke_sb.sb;
	}

	return &joke_sb.sb;
}

static void joke_kill_sb(struct super_block * sb) {
	panic("joke sb deallocated");
}

static int joke_mkdir(struct node * i, const char * name, size_t nlen, int flags) {
	//debug_print_line("joke_mkdir");
	virtual_node_create_as_child(name, nlen, i);
	return 0;
}

static int joke_rmdir(struct node * i) {
	//debug_print_line("joke_rmdir");
	virtual_node_release(i);
	return 0;
}

static int joke_iterate(struct file * filp, void * priv) {
	struct dirent * de = (struct dirent *) priv;
	struct node * parent;
	struct node * d;
	struct dlist_head * it;

	if (filp->f_node->directory_flag == 0) 
		return ENOTSUP;

	parent = filp->f_node;
	it = parent->childrens.next;//dlist_entry_first(&parent->childrens, struct dentry, lnk);

	int n = filp->pos;
	while(n--) it = it->next;
	filp->pos++;

	if (it == &parent->childrens) de->d_name[0] = '\0';
	else {
		d = dlist_entry(it, struct node, lnk);
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

const struct node_operations joke_i_op = {
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