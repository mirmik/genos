#ifndef GENOS_PATH_H
#define GENOS_PATH_H

struct vfsmount;
struct dentry;

struct path {
	struct vfsmount *mnt;
	struct dentry *dentry;	
};

void path_get(const struct path *);
void path_put(const struct path *);

static inline path_equal(const struct path* path1, const struct path* path2)
{
	return path1->mnt == path2->mnt && path1->dentry == path2->dentry;
}

#endif