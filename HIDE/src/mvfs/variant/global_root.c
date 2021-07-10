#include <mvfs/mvfs.h>

static struct node* __global_root;

struct node * vfs_get_root() {
	return __global_root;
}

void vfs_set_global_root(struct node * d) {
	__global_root = d;
}

void vfs_set_root(struct node * d) {
	__global_root = d; 
}