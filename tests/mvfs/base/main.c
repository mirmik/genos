#include <mvfs/mvfs.h>
#include <mvfs/fs/joke/joke.h>

#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/virtual/zero.h>
#include <drivers/cdev/virtual/debug.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
	int sts;

	vfs_register_fs(&joke_fstype);
	vfs_mount_first("joke", 0, NULL);

	sts = vfs_mkdir("/dev");
		if (sts) dprf("%s\n", strerror(sts));

	sts =link_null_device("/dev");
		if (sts) dprf("%s\n", strerror(sts));

	sts =link_zero_device("/dev");
		if (sts) dprf("%s\n", strerror(sts));

	sts =link_debug_device("/dev");
		if (sts) dprf("%s\n", strerror(sts));

	vfs_dpr_node_tree(NULL);

	struct file * filp; 
	sts = vfs_open("/dev/debug", 0, &filp);
		if (sts) dprf("%s\n", strerror(sts));

	vfs_write(filp, "HelloWorld\n", 11);
}