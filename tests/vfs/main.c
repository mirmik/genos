#include <mvfs/mvfs.h>
#include <mvfs/joke/fs.h>
#include <mvfs/fsops.h>

#include <gxx/debug/dprint.h>

#include <errno.h>

#include <mvfs/fsops.h>
#include <mvfs/variant/global_root.h>

#include <stdio.h>

int main() {
	mvfs_register_fs(&joke_fstype);

	//mvfs_mount_first("joke", 0, NULL);
	//mvfs_mkdir("/dev");

	struct dentry * a = mvfs_dentry_create("/");
	struct dentry * b = mvfs_dentry_create("dev");
	struct dentry * c = mvfs_dentry_create("home");
	struct dentry * d = mvfs_dentry_create("mirmik");
	struct dentry * e = mvfs_dentry_create("project");

	mvfs_dentry_add_child(b, a);
	mvfs_dentry_add_child(c, a);
	mvfs_dentry_add_child(d, c);
	mvfs_dentry_add_child(e, d);

	struct dentry * it;

	it = mvfs_dentry_lookup_child(a, "dev", 3);
	dprln(it ? it->name : "NONE");

	it = mvfs_dentry_lookup_child(c, "mirmik", 6);
	dprln(it ? it->name : "NONE");

	const char* path = "/home/mirmik/project";
	const char* lpath;
	struct dentry * resd = a;

	int sts = mvfs_lookup(path, &lpath, &resd);
	printf("status: %d\n", sts); 

	if (sts == 0) {
		printf("result: %s\n", resd->name); 		
	}
}