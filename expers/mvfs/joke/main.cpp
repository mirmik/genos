#include <mvfs/mvfs.h>
#include <mvfs/fs/joke/joke.h>
#include <mvfs/file.h>

#include <drivers/cdev/virtual/debug.h>
#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/virtual/zero.h>

#include <stdio.h>

int main()
{
	struct file * dfile;
	struct file * f;
	int sts;

	dprln("registry fs:");
	vfs_register_fs(&joke_fstype);
	vfs_mount_first("joke", 0, NULL);


	dprln("mkdir:");
	if (sts = vfs_mkdir("dev"))
		dprf("error: %s\n", strerror(sts));
	
	sts = vfs_mkdir("/home");
	if (sts)
		dprf("error: %s\n", strerror(sts));

	sts = vfs_mkdir("/home/mirmik");
	if (sts)
		dprf("error: %s\n", strerror(sts));

	sts = vfs_mkdir("/home/mirmik/project");
	if (sts)
		dprf("error: %s\n", strerror(sts));

	sts = vfs_mkdir("/home/mirmik/project/genos");
	if (sts)
		dprf("error: %s\n", strerror(sts));

	if (sts = vfs_rmdir("/home/mirmik/project/genos"))
	{
		dprf("error: %s\n", strerror(sts));
	}

	vfs_debug_tree("/");

	dprln("link devices:");
	link_debug_device("/dev");
	link_zero_device("/dev");
	link_null_device("/dev");

	vfs_debug_tree("/");

	dprln("open:");
	sts = vfs_open("/dev/debug", 0, &dfile);
	if (sts) dprf("error: %s\n", strerror(sts));

	dprln("write:");
	sts = vfs_write(dfile, "HelloWorld\n", 11);
	if (sts < 0) dprf("error: %s\n", strerror(errno));

	dprln("chdir && pwd");
	vfs_chdir("/home/mirmik");
	vfs_debug_pwd();
	vfs_chdir("../..");
	vfs_debug_pwd();

	dprln("remove:");
	sts = vfs_rmdir("/home/mirmik/project");
	sts = vfs_rmdir("/home/mirmik/models");
	sts = vfs_rmdir("/home/mirmik");
	sts = vfs_rmdir("/home/ggg");
	sts = vfs_rmdir("/home");

	vfs_debug_tree("/");
}