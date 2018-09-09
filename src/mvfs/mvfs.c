#include <mvfs/mvfs.h>

#include <mvfs/vfsmount.h>


void mvfs_init() {
	//stable_init(&mvfs_vfsmount_hashtable);
}

/**
 *	lookup для простых виртуальных fs.
 *
 *	Т.к. система держит всю информацию в памяти, просто говорим, что данных нет.
 *	Можно создать негативный dentry, но с целью экономии памяти, просто ничего не делаем.
 */
struct dentry * mvfs_virtual_lookup(struct inode * i, const char* name, unsigned int nlen) {
	(void) i; (void) name; (void) nlen;
	return NULL;
}

int mvfs_lookup(const char* str_path, struct dentry** result) {
	*result = mvfs_get_root();
}