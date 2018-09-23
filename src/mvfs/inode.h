#ifndef MVFS_INODE_H
#define MVFS_INODE_H

#include <stdint.h>

struct file;

/// Индексный дескриптор файла.
struct inode {
	struct super_block *      i_sb; ///Указатель на суперблок, к которому относится данный inode.
	uint16_t                  nlink;
	union {
		uint8_t flags; 
		struct {
			uint8_t directory_flag : 1;
		};
	};
};

///Один из специальных типов файлов.
struct special_inode {
	struct inode i;
	const struct file_operations* f_op;
};

struct inode_operations {
//super_block operations:
	//struct inode *(*alloc_inode)(struct super_block *sb); ///< Выделить inode с приватными данными, специфическими для fs.
	//int           (*destroy_inode)(struct inode *inode); ///< Парная предыдущей операция деаллокации.

//inode operations: 
	///Создать inode для переданной dentry
	//int (*create) (struct inode *, struct dentry *, uint8_t);

	///Процедура lookup. В отличие от большого vfs, с целью экономии памяти может ничего не возвращать.
	///Принимает родительский inode. Может создать пару inode-dentry, или вернуть негативный dentry.
	struct dentry * (*lookup) (struct inode *, struct dentry *);

	///Оторвать dentry от inode.
	//int (*unlink) (struct inode *,struct dentry *);

	//revalidate - вопрос на закэшированную запись.


    int (*mkdir) (struct inode *, struct dentry *,int);
    int (*rmdir) (struct inode *, struct dentry *);
};

__BEGIN_DECLS

extern int vfs_open_inode(struct inode * i, struct file ** filpp);

__END_DECLS

#endif