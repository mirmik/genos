/**
 *	@file 
 *	Сюда вынесены функции, которые необходимо реализовать для совместимости 
 *	с окружением.
 */

#ifndef MVFS_COMPAT_H
#define MVFS_COMPAT_H

__BEGIN_DECLS

/// Impl. Получить текущий root
extern struct dentry* mvfs_get_root();
extern void mvfs_set_root(struct dentry*);

/// Impl. Получить текущий путь.
extern struct dentry* mvfs_get_pwd();
extern void mvfs_set_pwd(struct dentry*);

/// Impl. Лок структур данных mvfs.
static inline void mvfs_lock() {}
static inline void mvfs_unlock() {}

extern struct file* mvfs_file_alloc();
extern void mvfs_file_dealloc(struct file*);

extern struct vfsmount * mvfs_vfsmount_alloc();
extern void mvfs_vfsmount_dealloc(struct vfsmount *);

extern struct dentry * mvfs_dentry_alloc();
extern void mvfs_dentry_dealloc(struct dentry *);

__END_DECLS

#endif