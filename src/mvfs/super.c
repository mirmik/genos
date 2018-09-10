#include <mvfs/super.h>
#include <gxx/panic.h>

#include <errno.h>

int mvfs_inode_lookup_child(struct inode * inode, 
	const char* name, unsigned int nlen, struct dentry ** retd
) {
	return -ENOTSUP;
	//panic("TODO: mvfs_inode_lookup_child");
}
