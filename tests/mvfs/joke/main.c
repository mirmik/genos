#include <mvfs/mvfs.h>
#include <mvfs/fs/joke/joke.h>
#include <mvfs/file.h>

#include <drivers/cdev/virtual/debug.h>
#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/virtual/zero.h>

#include <stdio.h>

int main() {
	int sts;

	vfs_register_fs(&joke_fstype);
	vfs_mount_first("joke", 0, NULL);

	if (sts = vfs_mkdir("dev")) {
		dprf("error: %s\n", strerror(sts));
	}

	vfs_mkdir("/home");
	vfs_mkdir("/home/mirmik");
	vfs_mkdir("/home/mirmik/project");
	vfs_mkdir("/home/mirmik/project/genos");

	if (sts = vfs_rmdir("/home/mirmik/project/genos")) {
		dprf("error: %s\n", strerror(sts));
	}

	vfs_dpr_dentry_tree(vfs_get_root());

	link_debug_device("/dev");
	link_zero_device("/dev");
	link_null_device("/dev");


	vfs_dpr_dentry_tree(vfs_get_root());

	struct file * dfile;

	sts = vfs_open("/dev/debug", 0, &dfile);
	if (sts) dprf("error: %s\n", strerror(sts));
	
	sts = vfs_write(dfile, "HelloWorld\n", 11);
	if (sts < 0) dprf("error: %s\n", strerror(errno));

	vfs_chdir("/home/mirmik");

	vfs_dpr_pwd();
	vfs_chdir("..");

	vfs_mkdir("ggg");
	vfs_chdir("ggg");
	vfs_dpr_pwd();

	vfs_dpr_dentry_tree(vfs_get_root());

	vfs_chdir("../..");
	vfs_dpr_pwd();

	struct file * f;
	sts = vfs_open("/dev", 0, &f);
	if (sts) dprf("error: %s\n", strerror(sts));

	dln();
	vfs_mkdir("/home/mirmik/models");
	vfs_debug_ls("/home/mirmik");

	dln();

	sts = vfs_rmdir("/home/mirmik/project");
	sts = vfs_rmdir("/home/mirmik/models");
	sts = vfs_rmdir("/home/mirmik");
	sts = vfs_rmdir("/home/ggg");
	sts = vfs_rmdir("/home");

	vfs_dpr_dentry_tree(vfs_get_root());
}