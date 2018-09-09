#include <mvfs/mvfs.h>

static struct dentry* __global_root;

struct dentry * mvfs_get_root() {
	return __global_root;
}

void mvfs_set_global_root(struct dentry * d) {
	__global_root = d; 
}