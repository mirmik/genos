#include <mvfs/mvfs.h>
#include <mvfs/pathops.h>
#include <mvfs/dentry.h>
#include <mvfs/super.h>
#include <mvfs/vfsmount.h>

#include <gxx/debug/dprint.h>
#include <gxx/util/iteration_counter.h>

#include "stdio.h"
#include "errno.h"
#include "assert.h"

void mvfs_init() {
	//stable_init(&mvfs_vfsmount_hashtable);
}

/**
 *	lookup для простых виртуальных fs.
 *
 *	Т.к. система держит всю информацию в памяти, просто говорим, что данных нет.
 *	Можно создать негативный dentry, но с целью экономии памяти, просто ничего не делаем.
 */
struct dentry * mvfs_virtual_lookup(struct inode * i, const char* name, unsigned int nlen) {
	(void) i; (void) name; (void) nlen;
	return NULL;
}


int mvfs_lookup_relative(const char* str_path, const char** pend, 
	struct dentry** current
) {
	int sts;
	const char* str;
	struct vfsmount* mount;
	unsigned int nlen;

	str = str_path;
	struct inode * ipos;
	struct dentry * pos = *current;

	while(str = path_next(str, &nlen)) {
		//Обходим дерево dentry.
		pos = mvfs_dentry_lookup_child(pos, str, nlen);

		if (pos == NULL) {
			//Если дентри не найден в дереве, пробуем запросить inode.
			//Последнего найденного.

			if (NULL == (ipos = (*current)->d_inode)) {
				//Если inode данного объекта не существует (negative dentry), 
				//просто говорим:				
				return -ENOENT;
			}

			//Если inode существует, просим его создать нам dentry.
			if (sts = mvfs_inode_lookup_child(ipos, str, nlen, &pos)) {
				return sts;
			}

			//Если он смог, подшиваем этот dentry в дерево и переходим на него.
			mvfs_dentry_add_child(pos, *current);
		} else {
			// Если дентри в дереве, надо проверить, не является ли он точкой монтирования.
			if (pos->mount_point_flag) {
				mount = mvfs_vfsmount_get(pos);
				assert(mount);
				//if (!mount) return -ENOKEY;
				pos = mount->root; 
			}
		}

		//Готовимся к следующей итерации алгоритма.
		//Если вернется ENOENT, неотработанная часть пути будет в pend.
		str += nlen;
		*current = pos;
		*pend = str;
	}

	//Путь полностью отработан. Дентри найден и вернется через поле current.
	return 0;
}