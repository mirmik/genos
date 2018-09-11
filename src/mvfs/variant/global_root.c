#include <mvfs/mvfs.h>

static struct dentry* __global_root;
static struct dentry* __global_pwd;

struct dentry * mvfs_get_root() {
	return __global_root;
}

struct dentry * mvfs_get_pwd() {
	return __global_pwd;
}

void mvfs_set_global_root(struct dentry * d) {
	__global_root = d; 
	__global_pwd = d; 
}

void mvfs_set_pwd(struct dentry * d) {
	__global_pwd = d; 
}

void mvfs_set_root(struct dentry * d) {
	__global_root = d; 
}