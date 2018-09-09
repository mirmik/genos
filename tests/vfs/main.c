#include <mvfs/mvfs.h>
#include <mvfs/joke/fs.h>
#include <mvfs/fsops.h>

#include <gxx/debug/dprint.h>

#include <errno.h>

#include <mvfs/fsops.h>
#include <mvfs/variant/global_root.h>

int main() {
	mvfs_register_fs(&joke_fstype);

	mvfs_mount_first("joke", 0, NULL);


	mvfs_mkdir("/dev");
}