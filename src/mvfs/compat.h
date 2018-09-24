/**
 *	@file 
 *	Сюда вынесены функции, которые необходимо реализовать для совместимости 
 *	с окружением.
 */

#ifndef MVFS_COMPAT_H
#define MVFS_COMPAT_H

__BEGIN_DECLS

/// Impl. Получить текущий root
extern struct node* vfs_get_root();
extern void vfs_set_root(struct node*);

/// Impl. Получить текущий путь.
extern struct node* vfs_get_pwd();
extern void vfs_set_pwd(struct node*);

/// Impl. Лок структур данных vfs.
static inline void vfs_lock() {}
static inline void vfs_unlock() {}

extern struct file* vfs_file_alloc();
extern void vfs_file_dealloc(struct file*);

extern struct vfsmount * vfs_vfsmount_alloc();
extern void vfs_vfsmount_dealloc(struct vfsmount *);

//extern struct dentry * vfs_dentry_alloc();
//extern void vfs_dentry_dealloc(struct dentry *);

__END_DECLS

#endif