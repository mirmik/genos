#include <mvfs/mvfs.h>

static struct node* __global_root;
static struct node* __global_pwd;

struct node * vfs_get_root() {
	return __global_root;
}

struct node * vfs_get_pwd() {
	return __global_pwd;
}

void vfs_set_global_root(struct node * d) {
	__global_root = d; 
	__global_pwd = d; 
}

void vfs_set_pwd(struct node * d) {
	__global_pwd = d; 
}

void vfs_set_root(struct node * d) {
	__global_root = d; 
}