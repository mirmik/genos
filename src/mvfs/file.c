#include <mvfs/file.h>
#include <mvfs/super.h>
#include <mvfs/dentry.h>
#include <mvfs/pathops.h>
#include <mvfs/mvfs.h>
#include <stdlib.h>

struct file * mvfs_file_alloc() {
	return (struct file*) malloc(sizeof(struct file));
}

void mvfs_file_destroy(struct file* filp) {
	free(filp);
}

int mvfs_open_with_root(const char* path, int flags, struct file** filp, 
	struct dentry * root
) {
	int sts;
	struct dentry * current;

	current = root;

	//Ищем dentry по соответствующему пути.
	if (sts = mvfs_lookup(path, &path, &current)) {
		return sts;
	}
	
	if (current->d_inode == NULL) {
		//dentry без inode. (или негативный, или добавленный вручную) 
		//Считаем, что файл не существует.
		return -ENOENT;
	}

	//Осталось сгенерировать file.
	*filp = mvfs_open_inode(current->d_inode);
	return 0;
}

int mvfs_open(const char* path, int flags, struct file** filp) {
	struct dentry * root = path_is_abs(path) ? mvfs_get_root() : mvfs_get_pwd();
	return mvfs_open_with_root(path, flags, filp, root);
}

int mvfs_close(struct file* filp) {
	return filp->f_op->release(filp->f_inode, filp);
}