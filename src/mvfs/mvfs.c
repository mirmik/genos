#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/dirent.h>
#include <mvfs/pathops.h>
#include <mvfs/variant/global_root.h>

#include <igris/dprint.h>
#include <igris/util/bug.h>

#include <errno.h>
#include <stdbool.h>
#include <string.h>

void vfs_init() {
	//stable_init(&vfs_vfsmount_hashtable);
}

int vfs_mount_first(const char* fstype, unsigned long mountflags, const void *data) 
{
	struct file_system_type * fs = vfs_get_fs(fstype);
	if (fs == NULL) 
		return ENOENT;

	struct super_block * sb = fs->fs_ops->get_sb(fs, mountflags, data);
	vfs_set_global_root(sb->root);

	return 0;
}

/*static inline int vfs_is_directory(struct node * d) 
{
	if (dlist_empty(&d->childrens)) {
		if (!d->directory_flag) {
			return false;
		}
	}
	return true;
}*/

int vfs_mount(const char *source, const char *target, 
	const char* fstype, unsigned long mountflags, const void *data
) {
	BUG();
	return 0;
}

int vfs_mkdir_do(struct node * parent, const char * name, size_t nlen, int f) 
{
	int sts;

	if (parent->n_ops->mkdir == NULL) 
		return ENOTSUP;

	sts = parent->n_ops->mkdir(parent, name, nlen, 0);
		if (sts)
			return sts;

	return 0;
} 

int vfs_mkdir(const char *path) 
{
	int sts;
	struct node * parent;
	const char * pend;

	sts = vfs_lookup(path, &pend, &parent);
		if (sts == 0) 
			return EEXIST;

	if (parent->flag.directory != 1) 
		return ENOTDIR;

	if (!path_is_simple(pend)) 
		return ENOTDIR;

	sts = vfs_mkdir_do(parent, pend, strlen(pend), 0);
		if (sts)
			return sts;

	return 0;
}

int vfs_rmdir_do(struct node * dir) 
{
	int sts;

	if (!dlist_empty(&dir->childs)) 
		return ENOTEMPTY;

	sts = dir->n_ops->rmdir(dir);
		if (sts)
			return sts;

	return 0;	
}

int vfs_rmdir(const char *path) 
{
	int sts;
	struct node * d;
	
	sts = vfs_lookup(path, NULL, &d);
		if (sts)
			return sts;

	if (d->flag.negative == 1) 
		return ENOENT;

	if (d->flag.directory == 0) 
		return ENOTDIR;

	return vfs_rmdir_do(d);
}

int vfs_chdir(const char *path) 
{
	struct node* tgt;
	int sts;

	sts = vfs_lookup(path, NULL, &tgt);
		if (sts)
			return sts;

	if (!tgt->flag.directory) 
		return ENOTDIR;

	vfs_set_pwd(tgt);
	return 0;
}

int vfs_chroot(const char *path) 
{
	struct node* tgt;
	int sts;

	sts = vfs_lookup(path, NULL, &tgt);
		if (sts) 
			return sts;


	if (!tgt->flag.directory) 
		return ENOTDIR;

	vfs_set_root(tgt);
	return 0;
}

int vfs_open(const char* path, int flags, struct file** filp) {
	int sts;
	struct node* d;	

	sts = vfs_lookup(path, NULL, &d);
		if (sts)
			return sts;

	if (d->flag.negative) {
		//dentry без inode. (или негативный, или добавленный вручную) 
		//Считаем, что файл не существует.
		return -ENOENT;
	}

	//Осталось сгенерировать file.
	sts = vfs_open_node(d, filp);
		if (sts)
			return sts;

	(*filp)->node = d;
	return 0;
}

int vfs_close(struct file* filp) {
	int sts;

	sts = filp->node->n_ops->release(filp->node, filp);
		if (sts) 
			return sts;

	return 0;
}

int vfs_write(struct file * filp, const char* data, unsigned int size) 
{
	errno = 0;
	return filp->node->n_ops->write(filp->node, filp, data, size);
}

int vfs_read(struct file * filp, char* data, unsigned int size) {
	errno = 0;
	return filp->node->n_ops->read(filp->node, filp, data, size);
}

int vfs_pwd(char* buf) 
{
	char* const bbuf = buf;
	int len;
	int flen = 0;
	char* cursor;

	if (vfs_get_pwd() == vfs_get_root()) {
		*buf++ = '/';
		*buf = '\0';
		return 1;
	}

	for (struct node * d = vfs_get_pwd();; d = d->parent) {
		if (d->parent == NULL) {
			if (d->flag.mount_point) {
				BUG();
			}
			break;
		}

		len = strlen(d->name);
		flen += len + 1;

		cursor = d->name + len;
		while(d->name != cursor) {
			--cursor;
			*buf++ = *cursor;
		}

		*buf++ = '/';
	}

	char* sit = bbuf;
	char* eit = bbuf + flen - 1;

	*buf = '\0';

	while(eit > sit) {
		char c;
		c = *eit;
		*eit = *sit;
		*sit = c;
		++sit;
		--eit;
	}

	return flen;
}

void vfs_debug_pwd() {
	char pwd[100];
	vfs_pwd(pwd);
	dprln(pwd);
}

int vfs_common_open (struct node * i, struct file * f) {
	(void) i; (void) f;
	//i->nlink++;
	return 0;
}

int vfs_common_release (struct node * i, struct file * f) {
	(void) i; (void) f;
	//i->nlink--;
	return 0;
}

int vfs_iterate(struct file * filp, struct dirent * de) {
	return filp->node->n_ops->iterate(filp->node, filp, de);
}

int vfs_debug_ls(const char* path) {
	int sts;
	struct node * d;
	struct dirent de;
	struct file * filp;

	if (path == NULL)
		d = vfs_get_pwd();
	else 
	{
		sts = vfs_lookup(path, NULL, &d);
		if (sts)
			return sts;
	}

	sts = vfs_open_node(d, &filp);
		if (sts)
			return sts;
	
	while(1) {
		sts = vfs_iterate(filp, &de);
		if (sts) {
			return sts;
			break;
		}
		if (*de.d_name == 0) break; 

		dprln(de.d_name);
	}

	vfs_close(filp);

	return 0;
}

int vfs_debug_tree(const char* path) {
	struct node * d;
	int sts;

	if (path == NULL)
		d = vfs_get_pwd();
	else
		sts = vfs_lookup(path, NULL, &d);
			if (sts)
				return sts;

	vfs_dpr_node_tree(d);

	return 0;
}