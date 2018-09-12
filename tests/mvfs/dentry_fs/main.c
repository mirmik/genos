#include <mvfs/mvfs.h>
#include <mvfs/joke/fs.h>
#include <mvfs/fsops.h>

#include <gxx/debug/dprint.h>

#include <errno.h>

#include <mvfs/fsops.h>
#include <mvfs/variant/global_root.h>
#include <mvfs/cdev.h>
#include <mvfs/file.h>
#include <mvfs/char/null.h>
#include <mvfs/char/zero.h>
#include <mvfs/char/debug.h>

#include <stdio.h>

int main() {
	mvfs_register_fs(&joke_fstype);

	struct dentry * a = mvfs_dentry_create("/");
	struct dentry * b = mvfs_dentry_create("dev");
	struct dentry * c = mvfs_dentry_create("home");
	struct dentry * d = mvfs_dentry_create("mirmik");
	struct dentry * e = mvfs_dentry_create("project");

	mvfs_dentry_add_child(b, a);
	mvfs_dentry_add_child(c, a);
	mvfs_dentry_add_child(d, c);
	mvfs_dentry_add_child(e, d);

	mvfs_set_global_root(a);

	link_null_device("/dev");
	link_zero_device("/dev");
	link_debug_device("/dev");

	int sts;
	struct file * f1;
	struct file * f2;
	struct file * f3;

	sts = mvfs_chdir("/dev");
	printf("chdir sts: %d\n", sts);

	sts = mvfs_open("null", 0, &f1);
	printf("open sts: %d\n", sts);

	sts = mvfs_open("zero", 0, &f2);
	printf("open sts: %d\n", sts);
	
	sts = mvfs_open("debug", 0, &f3);
	printf("open sts: %d\n", sts);

	mvfs_write(f1, "HelloWorld\n", 11);
	mvfs_write(f3, "HelloWorld\n", 11);
	mvfs_close(f3);

	sts = mvfs_chdir("/dev");
	printf("chdir sts: %d\n", sts);

	printf("res %d\n", a == mvfs_get_pwd());
	printf("res %d\n", a == mvfs_get_root());
}