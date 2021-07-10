#ifndef GENOS_MVFS_MVFS_H
#define GENOS_MVFS_MVFS_H

/**
 *	Попытка написать минимальный vfs.
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <mvfs/compat.h>
#include <mvfs/pathops.h>

#include <mvfs/fstype.h>
#include <mvfs/dirent.h>
#include <mvfs/super.h>

__BEGIN_DECLS

//////////////////////////////////////////API///////////////////////////////////////////////

/// Инициализация структур данных vfs.
extern void vfs_init();

/// Примонтировать файловую систему fstype в корень vfs
extern int vfs_mount_first(const char* fsname, unsigned long mountflags, const void *data);

/// Монтировать...
extern int vfs_mount(const char *source, const char *target, 
	const char* fsname, unsigned long mountflags, const void *data);

///Провести процедуру lookup по относительному пути.
extern int vfs_lookup(const char *str_path, const char **pend, struct node** current);

extern int vfs_mkdir(const char *path);
extern int vfs_rmdir(const char *path);

/// Взять следующее имя из директории. 
/// @de - хранит результат и номер итерации. 
extern int vfs_iterate(struct file * filp, struct dirent * de);

/// Стандартные операции над файлами.
extern int vfs_open(const char* path, int flags, struct file **filpp);
extern int vfs_close(struct file *filpp);
extern int vfs_write(struct file *filp, const char* data, unsigned int size, int flags);
extern int vfs_read(struct file *filp, char* data, unsigned int size, int flags);

/// Изменить текущий нод на нод, соответствующий path
extern int vfs_chdir(const char *path);

/// Изменить текущий корень на нод, соответствующий path
extern int vfs_chroot(const char *path);

/// Считать текущее положение
extern int vfs_pwd(char* buf);

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// DEBUG////////////////////////////////////////////////

extern void vfs_debug_pwd();
extern int vfs_debug_ls(const char *path);
extern int vfs_debug_tree(const char *path);

/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////Локальные утилиты//////////////////////////////////////////

/// Установить node в качестве глобального корня.
extern void vfs_set_global_root(struct node *d);

/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////Common implementations/////////////////////////////////////

extern int vfs_common_open (struct node *, struct file *);
extern int vfs_common_release (struct node *, struct file *);	

/////////////////////////////////////////////////////////////////////////////////////////////

__END_DECLS

#endif