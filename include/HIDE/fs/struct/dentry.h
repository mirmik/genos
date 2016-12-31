#ifndef GENOS_DENTRY_H
#define GENOS_DENTRY_H

#define DENTRY_NAME_LEN 9

struct dentry {
	char name[DENTRY_NAME_LEN];
	int flags;

	atomic_t refs;

	struct inode 		*d_inode;
//	struct super_block 	*d_sb;

	struct dentry     *parent;
	struct dlist_head children; /* Subelements of directory */
	struct dlist_head children_lnk;
};

#endif