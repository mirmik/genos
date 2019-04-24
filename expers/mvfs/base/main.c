#include <mvfs/mvfs.h>
#include <mvfs/fs/joke/joke.h>

#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/virtual/zero.h>
#include <drivers/cdev/virtual/debug.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <igris/dprint.h>

int main() {
	int sts;

	
	vfs_register_fs(&joke_fstype.fs);

	dprln("step 1");
	sts = vfs_mount_first("joke", 0, NULL);
		if (sts) dprln(strerror(sts));

	vfs_debug_pwd();
	//vfs_debug_ls(NULL);
	vfs_debug_tree(NULL);

	dprln("step 2");
	sts = vfs_mkdir("/dev");
		if (sts) dprln(strerror(sts));

	dprln("step 3");
	sts =link_null_device("/dev");
		if (sts) dprln(strerror(sts));

	dprln("step 4");
	sts =link_zero_device("/dev");
		if (sts) dprln(strerror(sts));

	dprln("step 5");
	sts =link_debug_device("/dev");
		if (sts) dprln(strerror(sts));

	dprln("step 6");
	vfs_dpr_node_tree(NULL);

	dprln("step 7");
	struct file * filp; 
	sts = vfs_open("/dev/debug", 0, &filp);
		if (sts) dprln(strerror(sts));

	dprln("step 8");
	sts = vfs_write(filp, "HelloWorld\n", 11, 0);
		if (sts < 0) dprln(strerror(sts));
}