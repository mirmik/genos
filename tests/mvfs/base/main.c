#include <mvfs/mvfs.h>
#include <mvfs/fs/joke/joke.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
	int sts;

	vfs_register_fs(&joke_fstype);
	vfs_mount_first("joke", 0, NULL);

	sts = vfs_mkdir("/dev");
	if (sts) dprf("%s\n", strerror(sts));

	sts = vfs_debug_ls(NULL);
	if (sts) dprf("%s\n", strerror(sts));

	vfs_dpr_node_tree(NULL);
}