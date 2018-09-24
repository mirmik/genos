#include <mvfs/mvfs.h>
#include <mvfs/pathops.h>
#include <mvfs/super.h>
#include <mvfs/vfsmount.h>

#include <gxx/debug/dprint.h>
#include <gxx/util/iteration_counter.h>

#include "string.h"
#include "stdio.h"
#include "errno.h"
#include "assert.h"

/**
 *	lookup для простых виртуальных fs.
 *
 *	Т.к. система держит всю информацию в памяти, просто говорим, что данных нет.
 */
struct node * vfs_virtual_lookup(struct node * i, const char * name) {
	(void) i; (void) name;
	return NULL;
}

struct node * vfs_dentry_lookup_child(struct node* parent, const char* name, unsigned int nlen) {
	/*struct node* node;

	if (nlen <= 2) {
		if (nlen == 1 && name[0] == '.') return parent;
		if (nlen == 2 && name[0] == '.' && name[1] == '.') {
			if (parent->parent)
				return parent->parent;
			else
				return parent;
		} 
	} 
	
	vfs_lock();
	dlist_for_each_entry(node, &parent->childrens, lnk) {
		int node_nlen = strlen(node->name);
		if (node_nlen == nlen && strncmp(name, node->name, nlen) == 0) return node;
	}
	vfs_unlock();

	return NULL;*/
}

int vfs_lookup_directory_child(struct node * dir, struct dentry * ddir, 
	const char* name, unsigned int nlen, struct dentry **dret) 
{
	/*int sts;
	struct dentry * ret;

	if (dir->i_sb->i_op->lookup == NULL) return ENOTSUP;

	ret = vfs_dentry_create_n(name, nlen);
	if (dir->i_sb->i_op->lookup(dir, ret) == NULL) {
		//Here we can link negative dentry.
		vfs_dentry_dealloc(ret);
		return ENOENT;
	}

	vfs_dentry_add_child(ret, ddir);
	return 0;*/
}

int vfs_lookup_relative(const char* str_path, const char** pend, 
	struct node** current
) {
	/*int sts;
	const char* str;
	struct vfsmount* mount;
	unsigned int nlen;

	str = str_path;
	struct node * ipos;
	struct dentry * pos = *current;

	while(str = path_next(str, &nlen)) {
		if (pend) *pend = str;

		//Обходим дерево dentry.
		pos = vfs_dentry_lookup_child(pos, str, nlen);

		if (pos == NULL) {
			//Если дентри не найден в дереве, пробуем запросить node.
			//Последнего найденного.

			if (NULL == (ipos = (*current)->d_node) || ipos->directory_flag == 0) {
				//Если node данного объекта не существует (negative dentry), или он не дирректорий 
				//просто говорим:				
				return ENOENT;
			}

			//Если node существует, пробуем создать новый dentry.
			if (sts = vfs_lookup_directory_child(ipos, *current, str, nlen, &pos)) {
				return sts;
			}
			//Если он смог, переходим на него.
		} else {
			// Если дентри в дереве, надо проверить, не является ли он точкой монтирования.
			if (pos->mount_point_flag) {
				mount = vfs_vfsmount_get(pos);
				assert(mount);
				//if (!mount) return -ENOKEY;
				pos = mount->root; 
			}
		}

		//Готовимся к следующей итерации алгоритма.
		//Если вернется ENOENT, неотработанная часть пути будет в pend.
		str += nlen;
		*current = pos;
	}

	//Путь полностью отработан. Дентри найден и вернется через поле current.
	return pos->d_node ? 0 : ENOENT;*/
}