#include <mvfs/mvfs.h>
#include <mvfs/fs/joke/joke.h>
#include <mvfs/file.h>

#include <drivers/cdev/virtual/debug.h>
#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/virtual/zero.h>

#include <stdio.h>

#include <igris/dprint.h>

int main()
{
	struct file * dfile;
	struct file * f;
	int sts;

	dprln("registry fs:");
	vfs_register_fs(&joke_fstype.fs);
	vfs_mount_first("joke", 0, NULL);


	dprln("mkdir /dev");
	if (sts = vfs_mkdir("dev"))
		dprln("error: ", strerror(sts));
	
	dprln("mkdir /home");
	sts = vfs_mkdir("/home");
	if (sts)
		dprln("error: ", strerror(sts));

	dprln("mkdir /home/mirmik");
	sts = vfs_mkdir("/home/mirmik");
	if (sts)
		dprln("error: ", strerror(sts));

	dprln("mkdir /home/mirmik/project");
	sts = vfs_mkdir("/home/mirmik/project");
	if (sts)
		dprln("error: ", strerror(sts));

	dprln("mkdir /home/mirmik/project/genos");
	sts = vfs_mkdir("/home/mirmik/project/genos");
	if (sts)
		dprln("error: ", strerror(sts));

	dprln("rmdir /home/mirmik/project/genos");
	if (sts = vfs_rmdir("/home/mirmik/project/genos"))
	{
		dprln("error: ", strerror(sts));
	}

	vfs_debug_tree("/");

	dprln("link devices:");
	link_debug_device("/dev");
	link_zero_device("/dev");
	link_null_device("/dev");

	vfs_debug_tree("/");

	dprln("open:");
	sts = vfs_open("/dev/debug", 0, &dfile);
	if (sts) dprln("error: ", strerror(sts));

	dprln("write:");
	sts = vfs_write(dfile, "HelloWorld\n", 11, 0);
	if (sts < 0) dprln("error: ", strerror(sts));

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