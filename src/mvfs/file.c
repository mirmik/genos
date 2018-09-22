#include <mvfs/file.h>
#include <mvfs/super.h>
#include <mvfs/dentry.h>
#include <mvfs/pathops.h>
#include <mvfs/mvfs.h>
#include <stdlib.h>

struct file * vfs_file_alloc() {
	return (struct file*) malloc(sizeof(struct file));
}

void vfs_file_dealloc(struct file* filp) {
	free(filp);
}