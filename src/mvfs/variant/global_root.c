#include <mvfs/mvfs.h>

static struct dentry* __global_root;
static struct dentry* __global_pwd;

struct dentry * vfs_get_root() {
	return __global_root;
}

struct dentry * vfs_get_pwd() {
	return __global_pwd;
}

void vfs_set_global_root(struct dentry * d) {
	__global_root = d; 
	__global_pwd = d; 
}

void vfs_set_pwd(struct dentry * d) {
	__global_pwd = d; 
}

void vfs_set_root(struct dentry * d) {
	__global_root = d; 
}