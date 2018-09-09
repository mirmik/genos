#ifndef MVFS_PATH_H
#define MVFS_PATH_H

struct path {
	struct dentry * d;
	struct vfsmount * m;
};

#endif