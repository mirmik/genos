#ifndef GENOS_MOUNT_H
#define GENOS_MOUNT_H

struct dentry;
struct super_block;

struct vfsmount {
	struct dentry *mnt_root;
	struct super_block *mnt_sb;
	unsigned char flags;
};

#endif