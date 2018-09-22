#include <mvfs/fsops.h>
#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/dentry.h>
#include <mvfs/file.h>
#include <mvfs/pathops.h>
#include <mvfs/variant/global_root.h>

#include <gxx/panic.h>
#include <gxx/debug/dprint.h>

#include <errno.h>
#include <stdbool.h>

void vfs_init() {
	//stable_init(&vfs_vfsmount_hashtable);
}

int vfs_mount_first(const char* fstype, unsigned long mountflags, const void *data) 
{
	struct file_system_type * fs = vfs_get_fs(fstype);

	struct super_block * sb = fs->get_sb(fs, mountflags, data);
	vfs_set_global_root(sb->s_root);

	return 0;
}

static inline int vfs_is_directory(struct dentry * d) 
{
	if (dlist_empty(&d->childrens)) {
		if (!d->d_inode || !d->d_inode->directory_flag) {
			return false;
		}
	}
	return true;
}

int vfs_mount(const char *source, const char *target, 
	const char* fstype, unsigned long mountflags, const void *data
) {
	panic("TODO");
}

int vfs_mkdir_do(struct inode * i, struct dentry * d, int f) 
{
	int sts;

	if (i->i_sb->i_op->mkdir == NULL) 
		return ENOTSUP;

	if (sts = i->i_sb->i_op->mkdir(i, d, 0)) {
		return sts;
	}

	return 0;
} 

int vfs_mkdir(const char *path) 
{

	int sts;
	struct dentry * d;
	struct dentry * parent;
	struct inode * i;
	const char * pend;

	sts = vfs_lookup(path, &pend, &parent);

	if (sts == 0) return EEXIST;

	i = parent->d_inode;
	if (i->directory_flag != 1) return ENOTDIR;
	if (!path_is_simple(pend)) return ENOTDIR;

	d = vfs_dentry_create(pend);

	if (sts = vfs_mkdir_do(i, d, 0)) {
		vfs_dentry_dealloc(d);
		return sts;
	}

	vfs_dentry_add_child(d, parent);
	return 0;
}

int vfs_rmdir_do(struct inode * i, struct dentry * d) 
{
	int sts;

	if (!dlist_empty(&d->childrens)) 
		return ENOTEMPTY;

	if (i->i_sb->i_op->rmdir == NULL) 
		return ENOTSUP;

	if (sts = i->i_sb->i_op->rmdir(i, d))
		return sts;

	vfs_dentry_clean(d);

	return 0;	
}

int vfs_rmdir(const char *path) 
{
	int sts;
	struct dentry * d;
	struct inode * i;

	if (sts = vfs_lookup(path, NULL, &d))
		return sts;

	i = d->d_inode;
	if (i == NULL) 
		return ENOENT;

	if (d->d_inode->directory_flag == 0) 
		return ENOTDIR;

	return vfs_rmdir_do(i, d);
}

int vfs_chdir(const char *path) 
{
	struct dentry* tgt;
	int sts;

	if (sts = vfs_lookup(path, NULL, &tgt)) {
		return sts;
	}

	if (!vfs_is_directory(tgt)) 
		return ENOTDIR;

	vfs_set_pwd(tgt);
	return 0;
}

int vfs_chroot(const char *path) 
{
	struct dentry* tgt;
	int sts;

	if (sts = vfs_lookup(path, NULL, &tgt)) {
		return sts;
	}

	if (!vfs_is_directory(tgt)) 
		return ENOTDIR;

	vfs_set_root(tgt);
	return 0;
}

int vfs_open(const char* path, int flags, struct file** filp) {
	int sts;
	struct dentry* d;	

	if (sts = vfs_lookup(path, NULL, &d)) {
		return sts;
	}

	if (d->d_inode == NULL) {
		//dentry без inode. (или негативный, или добавленный вручную) 
		//Считаем, что файл не существует.
		return -ENOENT;
	}

	//Осталось сгенерировать file.
	if (sts = vfs_open_inode(d->d_inode, filp))
		return sts;

	(*filp)->f_dentry = d;
	return 0;
}

int vfs_close(struct file* filp) {
	int sts;

	sts = filp->f_op->release(filp->f_inode, filp);
	if (sts) return sts;
}

int vfs_write(struct file * filp, const char* data, unsigned int size) {
	errno = 0;

	if (filp->f_op->write == NULL) 
		return ENOTSUP;
	
	return filp->f_op->write(filp, data, size);
}

int vfs_read(struct file * filp, char* data, unsigned int size) {
	errno = 0;

	if (filp->f_op->read == NULL) 
		return ENOTSUP;
	
	return filp->f_op->read(filp, data, size);
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

	for (struct dentry * d = vfs_get_pwd();; d = d->parent) {
		if (d->parent == NULL) {
			if (d->mount_point_flag) {
				panic("TODO");
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

void vfs_dpr_pwd() {
	char pwd[100];
	vfs_pwd(pwd);
	dprln(pwd);
}

int vfs_common_open (struct inode * i, struct file * f) {
	(void) i; (void) f;
	i->nlink++;
	return 0;
}

int vfs_common_release (struct inode * i, struct file * f) {
	(void) i; (void) f;
	i->nlink--;
	return 0;
}

int vfs_iterate(struct file * filp, struct dirent * de) {
	return filp->f_op->iterate(filp, de);
}

int vfs_debug_ls(const char* path) {
	int sts;
	struct dentry * d;
	struct dirent de;
	struct file * filp;

	if (path == NULL)
		d = vfs_get_pwd();
	else
		if (sts = vfs_lookup(path, NULL, &d))
			return sts;
	if (sts = vfs_open_inode(d->d_inode, &filp))
		return sts;
	filp->f_dentry = d;

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