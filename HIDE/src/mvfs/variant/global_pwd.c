#include <mvfs/mvfs.h>

static struct node* __global_pwd;

void vfs_set_pwd(struct node * d) {
	__global_pwd = d; 
}

struct node * vfs_get_pwd() {
	if (__global_pwd == NULL) __global_pwd = vfs_get_root();
	return __global_pwd;
}