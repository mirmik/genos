#include <mvfs/mvfs.h>
#include <mvfs/pathops.h>
#include <mvfs/super.h>
#include <mvfs/vfsmount.h>

#include <igris/dprint.h>
//#include <igris/util/iteration_counter.h>

#include "string.h"
#include "stdio.h"
#include "errno.h"
#include "assert.h"

/**
 *	lookup для простых виртуальных fs.
 *
 *	Т.к. система держит всю информацию в памяти, просто говорим, что данных нет.
 */
struct node * vfs_virtual_lookup(struct node * i, const char * name, size_t nlen) {
	(void) i; (void) name;
	return NULL;
}

int vfs_lookup_child(struct node** pos, 
	const char* name, unsigned int nlen) 
{
	struct node* it;
	struct node * p = *pos;

	if (p->flag.negative || p->flag.directory == 0) {
		//Если node данного объекта не существует (negative dentry), или он не дирректорий 
		//просто говорим:	
		return ENOENT;
	}

	if (nlen <= 2) {
		if (nlen == 1 && name[0] == '.') 
			return 0;
		
		if (nlen == 2 && name[0] == '.' && name[1] == '.') 
		{
			if (p->parent != NULL)
				*pos = p->parent;
			return 0;
		} 
	}

	vfs_lock();
	dlist_for_each_entry(it, &p->childs, lnk) {
		int node_nlen = strlen(it->name);
		if (node_nlen == nlen && strncmp(name, it->name, nlen) == 0) {
			*pos = it;
			return 0;
		}
	}
	//}
	vfs_unlock();

	//TODO: node lookup.
	return ENOENT;
}

int vfs_lookup_relative(const char* str_path, const char** pend, 
	struct node** current
) {
	int sts;
	unsigned int nlen;
	const char* str;
	struct node * pos;
	struct vfsmount* mount;
	
	str = str_path;
	pos = *current;
	if (pend) 
		*pend = str;

	while((str = path_next(str, &nlen)))
	{
		if (pend) 
			*pend = str;

		// Обходим дерево dentry.
		sts = vfs_lookup_child(&pos, str, nlen);
			if (sts)
				return sts;

		// Если он смог, переходим на него.
		// Если дентри в дереве, надо проверить, не является ли он точкой монтирования.
		if (pos->flag.mount_point) 
		{
			mount = vfs_vfsmount_get(pos);
			assert(mount != NULL);
			//if (!mount) return -ENOKEY;
			pos = mount->root; 
		}

		//Готовимся к следующей итерации алгоритма.
		//Если вернется ENOENT, неотработанная часть пути будет в pend.
		str += nlen;
		*current = pos;
	}

	//Путь полностью отработан. Дентри найден и вернется через поле current.
	return 0;
}