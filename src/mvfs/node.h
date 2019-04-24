#ifndef MVFS_NODE_H
#define MVFS_NODE_H

#include <igris/datastruct/dlist.h>
#include <stdint.h>
#include <string.h>

#include <errno.h>

#define NAME_LENGTH_MAX 7

#define NODE_NEGATIVE 0x01
#define NODE_DIRECTORY 0x02
#define NODE_SPECIAL 0x04

struct super_block;
struct file_operations;
struct file;

struct node;
struct node_operations
{
	//file operations
	int (*open) (struct node *, struct file*); ///< заполнить структуру файла по inode.
	int (*release) (struct node *, struct file*); ///< вернуть структуру файла в распределитель.
	int (*write) (struct node *, struct file*, const char* data, unsigned int size, int flags); ///< запись данных
	int (*read) (struct node *, struct file*, char* data, unsigned int size, int flags); ///< чтение данных
	int (*iterate) (struct node *, struct file*, void*); ///< чтение директории

	//inode operations
	int (*mkdir)(struct node *, const char * name, size_t nlen, int flags); ///< создать подчинённую директорию
	int (*rmdir)(struct node *); ///< Удалить данную директорию.
};

struct node
{
	struct dlist_head lnk;
	struct dlist_head childs;

	struct node * parent;
	char name[NAME_LENGTH_MAX + 1];
	struct super_block * sb;

	const struct node_operations * n_ops;

	union
	{
		uint8_t flags;
		struct
		{
			uint8_t negative : 1;
			uint8_t directory : 1;
			uint8_t special_node : 1;
			uint8_t mount_point : 1;
			uint8_t noblock : 1;
			uint8_t nosched : 1;
		} flag;
	};
};

#define NODE_INIT(name, cname, sb, ops, flags) 					\
	{ DLIST_HEAD_INIT(name.lnk), DLIST_HEAD_INIT(name.childs),	\
	NULL, cname, sb, ops, flags }

/*struct special_node {
	struct node node;
	const struct file_operations * f_op;
};

struct node_operations {
	struct node * (*lookup) (struct node * dir, const char * name, size_t nlen);
};*/

__BEGIN_DECLS

extern int vfs_open_node(struct node * i, struct file ** filpp);

//struct node * virtual_node_alloc();
//void virtual_node_dealloc(struct node *);

void node_init(struct node * node, const char * name, size_t nlen);

void node_add_child(struct node * node, struct node * parent);

extern void vfs_dpr_node_tree(struct node * root);

__END_DECLS

#endif