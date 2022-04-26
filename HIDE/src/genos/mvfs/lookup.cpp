#include <genos/mvfs/vfsnode.h>
#include <genos/mvfs/pathops.h>
#include <genos/mvfs/super.h>

#include <igris/dprint.h>

#include "string.h"
#include "stdio.h"
#include "errno.h"
#include "assert.h"

int vfs_lookup_child(struct node** pos,
                     const char* name, unsigned int nlen)
{
	struct node* it;
	struct node * p = *pos;

	if (p->flag.negative || p->flag.directory == 0)
	{
		//Если node данного объекта не существует (negative dentry), или он не дирректорий
		//просто говорим:
		return ENOENT;
	}

	if (nlen <= 2)
	{
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
	dlist_for_each_entry(it, &p->childs, lnk)
	{
		int node_nlen = strlen(it->name);

		if (node_nlen == nlen && strncmp(name, it->name, nlen) == 0)
		{
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
                       )
{
	int sts;
	unsigned int nlen;
	const char* str;
	struct node * pos;
	struct super_block * mountsb;

	str = str_path;
	pos = *current;

	if (pend)
		*pend = str;

	while ((str = path_next(str, &nlen)))
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
			mountsb = vfs_vfsmount_get(pos);
			assert(mountsb != NULL);
			pos = mountsb->root;
		}

		//Готовимся к следующей итерации алгоритма.
		//Если вернется ENOENT, неотработанная часть пути будет в pend.
		str += nlen;
		*current = pos;
	}

	//Путь полностью отработан. Дентри найден и вернется через поле current.
	return 0;
}

int vfs_lookup(const char* str_path, const char** pend, struct node** current)
{
	if (path_is_abs(str_path))
	{
		*current = vfs_get_root();
	}
	else
	{
		*current = vfs_get_pwd();
	}

	return vfs_lookup_relative(str_path, pend, current);
}