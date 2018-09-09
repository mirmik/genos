#include <fs/vfs.h>
#include <fs/node.h>
#include <fs/mount.h>
#include <fs/path.h>
#include <fs/hlpr_path.h>

#include <assert.h>
#include <string.h>

#include <gxx/datastruct/tree.h>
#include <gxx/panic.h>

#include <gxx/debug/dprint.h>

#define ROOT_MODE 0775
#define ERR_CHILD_NOT_FOUND 1
#define ERR_CHILD_MOUNTED   2

static int __vfs_subtree_lookup_existing(struct node *parent,
		const char *str_path, const char **p_end_existent, struct node **child);

struct node *vfs_create_root(void) {
	debug_print_line(__PRETTY_FUNCTION__);
	struct node *root_node;

	root_node = node_alloc("/", 0);
	assert(root_node);
	root_node->mode = S_IFDIR | ROOT_MODE;

	return root_node;
}

struct node *vfs_get_root(void) {
	debug_print_line(__PRETTY_FUNCTION__);
	static struct node *root_node;

	if (!root_node) {
		root_node = vfs_create_root();
	}

	return root_node;
}

void vfs_get_root_path(struct path *path) {
	debug_print_line(__PRETTY_FUNCTION__);
	path->node = vfs_get_root();
	path->mnt_desc = mount_table();
}

static struct node *__vfs_get_parent(struct node *child) {
	debug_print_line(__PRETTY_FUNCTION__);
	return tree_element(child->tree_link.par, struct node, tree_link);
}

struct node *vfs_subtree_lookup_childn(struct node *parent, const char *name, size_t len) {
	debug_print_line(__PRETTY_FUNCTION__);
	//struct lookup_tuple lookup = { .name = name, .len = len };
	//struct tree_link *tlink;
	struct node *node;

	//Если следующий элемент - двойная точка - возвращаем родителя, если он существует.
	if (path_is_double_dot(name))
		return (node = __vfs_get_parent(parent)) ? node : parent;

	//tlink = tree_lookup_child(&(parent->tree_link), vfs_lookup_cmp, &lookup);
	tree_for_each_child_entry(node, &parent->tree_link, tree_link) {
		if (strncmp(name, node->name, len) == 0) return node;
	}

	return NULL;
}

/*static int __vfs_subtree_lookup_existing(struct node *parent,
		const char *str_path, const char **p_end_existent,
		struct node **child_ptr
) {
	debug_print_line(__PRETTY_FUNCTION__);
	size_t len = 0;
	int res = 0;
	struct node *child = *child_ptr;

	assert(parent && str_path);

	while ((str_path = path_next(str_path, &len))) {
		child = vfs_subtree_lookup_childn(parent, str_path, len);
		if (!child) {
			res = -ERR_CHILD_NOT_FOUND;
			goto out;
		}

		str_path += len;
		parent = child;

		if (child->mounted) {
			res = -ERR_CHILD_MOUNTED;
			goto out;
		}
	}

	out: if (p_end_existent) {
		*p_end_existent = str_path;
	}

	*child_ptr = parent;
	return res;*/
//}

/**
 *	Рабочая лошадка. Ищет нод по пути.
 *	parent - точка начала поиска.
 *	str_path - строка пути	
 *	p_end_existent - ???
 *  path - сюда вернется результат
 */
static void __vfs_lookup_existing(const struct path *parent, 
		const char *str_path, const char **p_end_existent, struct path *path
) {
	debug_print_line(__PRETTY_FUNCTION__);
	struct node *node;
	size_t len = 0;

	//assert(parent && str_path);
	*path = *parent;
	node = parent->node;

	while (str_path = path_next(str_path, &len)) {
		node = vfs_subtree_lookup_childn(node, str_path, len);
		if (!node) break;
		
		//__vfs_subtree_lookup_existing(path->node, str_path, p_end_existent, &node);



		/*if_mounted_follow_down(path);
		if (-ERR_CHILD_MOUNTED
				!= __vfs_subtree_lookup_existing(path->node, str_path,
						p_end_existent, &node)) {
			path->node = node;
			break;
		}*/

		//str_path = *p_end_existent;
	}
	path->node = node;

	return;
}


void if_mounted_follow_down(struct path *path) {
	debug_print_line(__PRETTY_FUNCTION__);
	if (path->node->mounted) {
		panic("TODO: mounted follow down");
		//path->mnt_desc = mount_table_get_child(path->mnt_desc, path->node);
		//assert(path->mnt_desc);
		//path->node = path->mnt_desc->mnt_root;
	}
}

int vfs_lookup(const char *str_path, struct path *path) {
	debug_print_line(__PRETTY_FUNCTION__);
	struct path parent;

	vfs_get_root_path(&parent);
	if_mounted_follow_down(&parent);

	__vfs_lookup_existing(&parent, str_path, &str_path, path);

	if (path_next(str_path, NULL)) {
		/* Have unresolved fragments in path. */
		return -1;
	}

	return 0;
}

int vfs_add_leaf(struct node *child, struct node *parent) {
	tree_add_link(&(parent->tree_link), &(child->tree_link));
	return 0;
}

static struct node *__vfs_subtree_create_child(struct node *parent, const char *name, 
	size_t len, mode_t mode
) {
	struct node *child = NULL;

	assert(parent);

	child = node_alloc(name, len);
	if (child) {
		child->mode = mode;
		//child->uid = getuid();
		//child->gid = getgid();

		vfs_add_leaf(child, parent);
	}

	return child;
}

struct node *vfs_subtree_create_child(struct node *parent, const char *name, mode_t mode) {
	return __vfs_subtree_create_child(parent, name, strlen(name), mode);
}

void vfs_create_child(struct path *parent, const char *name, mode_t mode, struct path *child) {
	assert(parent);
	assert(mode & S_IFMT);
	//assertf(mode & S_IFMT, "Must provide a type of node, see S_IFXXX");

	if_mounted_follow_down(parent);

	child->node = vfs_subtree_create_child(parent->node, name, mode);
}




/*static int __vfs_create(struct path *parent, const char *path, mode_t mode,
		int intermediate, struct path *child) {
	assert(parent);
	assert(child);

	*child = *parent;

	__vfs_lookup_existing(child, path, &path, child);
	if_mounted_follow_down(child);

	child->node = __vfs_subtree_create(child->node, path, mode, intermediate);

	return child->node ? 0 : -1;
}

int vfs_create(struct path *parent, const char *path, mode_t mode,
		struct path *child) {
	return __vfs_create(parent, path, mode, 0, child);
}*/



/*
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include <fs/hlpr_path.h>
#include <fs/vfs.h>

#include <limits.h>

#include <sys/stat.h>
#include <assert.h>



static struct node *__vfs_get_parent(struct node *child);
static int __vfs_subtree_lookup_existing(struct node *parent,
		const char *str_path, const char **p_end_existent, struct node **child);
static struct node * __vfs_subtree_create(struct node *parent, const char *path,
		mode_t mode, int intermediate);
static struct node *__vfs_subtree_create_child(struct node *parent, const char *name,
		size_t len, mode_t mode);

struct lookup_tuple {
	const char *name;
	size_t len;
};

static int vfs_lookup_cmp(struct tree_link *link, void *data) {
	struct lookup_tuple *lookup = data;
	node_t *node = tree_element(link, node_t, tree_link);
	const char *name = node->name;
	return !(strncmp(name, lookup->name, lookup->len) || name[lookup->len]);
}

/*================================================================================
 ====================== Functions that are working with path ======================
 ==================================================================================*/
/*
int vfs_get_pathbynode_tilln(struct path *node, struct path *parent, char *path,
		size_t plen) {
	char *p;
	size_t ll = plen - 1;

	if (plen <= 0) {
		return -ERANGE;
	}

	p = path + ll;
	*p = '\0';

	while (node->node != parent->node && node->node != NULL) {
		size_t nnlen;

		if_root_follow_up(node);
		nnlen = strlen(node->node->name);

		if (nnlen + 1 > ll) {
			return -ERANGE;
		}

		p = strncpy(p - nnlen, node->node->name, nnlen);
		*--p = '/';
		ll -= nnlen + 1;

		vfs_get_parent(node, node);
	}

	memmove(path, p, plen - ll);

	if (node->node != parent->node) {
		return 1;
	}

	return 0;
}

void vfs_lookup_childn(struct path *parent, const char *name, size_t len,
		struct path *child) {
	assert(parent);
	assert(child);

	if_mounted_follow_down(parent);
	*child = *parent;

	child->node = vfs_subtree_lookup_childn(parent->node, name, len);
}

static void __vfs_lookup_existing(const struct path *parent,
		const char *str_path, const char **p_end_existent, struct path *path) {
	struct node *node;
	size_t len = 0;

	assert(parent && str_path);
	*path = *parent;

	while ((str_path = path_next(str_path, &len))) {
		if_mounted_follow_down(path);
		if (-ERR_CHILD_MOUNTED
				!= __vfs_subtree_lookup_existing(path->node, str_path,
						p_end_existent, &node)) {
			path->node = node;
			break;
		}

		str_path = *p_end_existent;
		path->node = node;
	}

	return;
}

void vfs_lookup_child(struct path *parent, const char *name, struct path *child) {
	vfs_lookup_childn(parent, name, strlen(name), child);
}

int vfs_lookup(const char *str_path, struct path *path) {
	struct path parent;

	vfs_get_root_path(&parent);

	if_mounted_follow_down(&parent);

	__vfs_lookup_existing(&parent, str_path, &str_path, path);

	if (path_next(str_path, NULL)) {
		/* Have unresolved fragments in path. */
/*		return -1;
	}

	return 0;
}

void vfs_create_child(struct path *parent, const char *name, mode_t mode,
		struct path *child) {
	assert(parent);
	assert(mode & S_IFMT);
	//assertf(mode & S_IFMT, "Must provide a type of node, see S_IFXXX");

	if_mounted_follow_down(parent);

	child->node = vfs_subtree_create_child(parent->node, name, mode);
}


int vfs_create_intermediate(struct path *parent, const char *path, mode_t mode,
		struct path *child) {
	return __vfs_create(parent, path, mode, 1, child);
}

int vfs_get_child_next(struct path *parent_path, struct node *child_prev, struct path *child_next) {
	*child_next = *parent_path;
	if_mounted_follow_down(child_next);

	child_next->node = vfs_subtree_get_child_next(child_next->node, child_prev);

	return child_next->node ? 0 : -1;
}

void vfs_get_parent(struct path *child_path, struct path *parent_path) {
	*parent_path = *child_path;
	if_root_follow_up(parent_path);
	parent_path->node = __vfs_get_parent(parent_path->node);
}

void vfs_get_root_path(struct path *path) {
	path->node = vfs_get_root();
	path->mnt_desc = mount_table();
}

void vfs_get_leaf_path(struct path *path) {
	char *leaf_name;

	if ((NULL == (leaf_name = getenv("PWD")))
			|| (0 != vfs_lookup(leaf_name, path))) {
		vfs_get_root_path(path);
	}
}


void if_root_follow_up(struct path *path) {
	if (path->node == path->mnt_desc->mnt_root) {
		path->node = path->mnt_desc->mnt_point;
		path->mnt_desc = path->mnt_desc->mnt_parent;
	}
}

/*================================================================================
 ====================== Functions that are working with node ======================
 ==================================================================================*/
/*

static int __vfs_subtree_lookup_existing(struct node *parent,
		const char *str_path, const char **p_end_existent,
		struct node **child_ptr) {
	size_t len = 0;
	int res = 0;
	struct node *child = *child_ptr;

	assert(parent && str_path);

	while ((str_path = path_next(str_path, &len))) {
		child = vfs_subtree_lookup_childn(parent, str_path, len);
		if (!child) {
			res = -ERR_CHILD_NOT_FOUND;
			goto out;
		}

		str_path += len;
		parent = child;

		if (child->mounted) {
			res = -ERR_CHILD_MOUNTED;
			goto out;
		}
	}

	out: if (p_end_existent) {
		*p_end_existent = str_path;
	}

	*child_ptr = parent;
	return res;
}

static struct node *__vfs_subtree_create(struct node *parent, const char *path,
		mode_t mode, int intermediate) {
	struct node *child = NULL;
	size_t len;
	struct node **tmp_parent;

	assert(parent);

	tmp_parent = &parent;

	__vfs_subtree_lookup_existing(*tmp_parent, path, &path, tmp_parent);

	path = path_next(path, &len);

	/* Here path points to the first non-existent fragment, if any. */

/*	if (intermediate) {
		const char *next_path;
		size_t next_len;

		if (!path) {
			/* Node already exist, set mode. */
			//XXX wtf?? parent->mode = mode;
/*			return *tmp_parent;
		}

		while ((next_path = path_next(path + len, &next_len))) {
			child = __vfs_subtree_create_child(*tmp_parent, path, len, S_IFDIR);

			if (!child) {
				return NULL;
			}

			tmp_parent = &child;
			path = next_path;
			len = next_len;
		}
	} else if (!path || path_next(path + len, NULL)) {
		/* Node already exists or missing intermediate node. */
/*		return NULL;
	}

	return vfs_subtree_create_child(*tmp_parent, path, mode);
}

static struct node *__vfs_subtree_create_child(struct node *parent, const char *name,
		size_t len, mode_t mode) {
	struct node *child = NULL;

	assert(parent);

	child = node_alloc(name, len);
	if (child) {
		child->mode = mode;
		child->uid = getuid();
		child->gid = getgid();

		vfs_add_leaf(child, parent);
	}

	return child;
}

struct node *vfs_subtree_create_child(struct node *parent, const char *name,
		mode_t mode) {
	return __vfs_subtree_create_child(parent, name, strlen(name), mode);
}

struct node *vfs_subtree_lookup_childn(struct node *parent, const char *name,
		size_t len) {
	struct lookup_tuple lookup = { .name = name, .len = len };
	struct tree_link *tlink;
	struct node *ret;

	assert(parent);

	if (path_is_double_dot(name))
		return (ret = __vfs_get_parent(parent)) ? ret : parent;

	tlink = tree_lookup_child(&(parent->tree_link), vfs_lookup_cmp, &lookup);

	return tree_element(tlink, struct node, tree_link);
}

struct node *vfs_subtree_lookup_child(struct node *parent, const char *name) {
	return vfs_subtree_lookup_childn(parent, name, strlen(name));
}

struct node *vfs_subtree_lookup(struct node *parent, const char *str_path) {
	struct node *node;

	assert(parent);

	__vfs_subtree_lookup_existing(parent, str_path, &str_path, &node);

	if (path_next(str_path, NULL)) {
		/* Have unresolved fragments in path. */
/*		return NULL;
	}

	return node;
}

struct node *vfs_subtree_get_child_next(struct node *parent, struct node *prev_child) {
	struct tree_link *chld_link;

	assert(parent);

	if (!prev_child) {
		chld_link = tree_children_begin(&parent->tree_link);
		goto out;
	}

	chld_link = tree_children_next(&prev_child->tree_link);

	if (tree_children_end(&parent->tree_link) == chld_link) {
		return NULL;
	}

out:
	return tree_element(chld_link, struct node, tree_link);
}

struct node *vfs_subtree_create(struct node *parent, const char *path,
		mode_t mode) {
	return __vfs_subtree_create(parent, path, mode, 0);
}

struct node *vfs_subtree_create_intermediate(struct node *parent,
		const char *path, mode_t mode) {
	return __vfs_subtree_create(parent, path, mode, 1);
}

node_t *vfs_get_leaf(void) {
	struct path leaf;

	vfs_get_leaf_path(&leaf);

	return leaf.node;
}

int vfs_del_leaf(node_t *node) {
	int rc;

	assert(node);

	rc = tree_unlink_link(&(node->tree_link));
	if (rc) {
		node_free(node);
	}
	return rc;
}

node_t *vfs_create_root(void) {
	node_t *root_node;

	root_node = node_alloc("/", 0);
	assert(root_node);
	root_node->mode = S_IFDIR | ROOT_MODE;

	return root_node;
}

node_t *vfs_get_root(void) {
	static node_t *root_node;

	if (!root_node) {
		root_node = vfs_create_root();
		//TODO set pseudofs driver
	}

	return root_node;
}

int vfs_add_leaf(node_t *child, node_t *parent) {
	tree_add_link(&(parent->tree_link), &(child->tree_link));
	return 0;
}

node_t *vfs_subtree_get_parent(node_t *node) {
	return __vfs_get_parent(node);
}

int vfs_get_relative_path(struct node *node, char *path, size_t path_len) {
	struct node *prev = NULL;
	char *p;
	size_t ll = path_len - 1;

	assert(path_len > 0);

	p = path + ll;
	*p = '\0';

	while (node != prev && node != NULL) {
		size_t nnlen;

		nnlen = strlen(node->name);

		if (nnlen + 1 > ll) {
			return -ERANGE;
		}

		p = strncpy(p - nnlen, node->name, nnlen);
		*--p = '/';
		ll -= nnlen + 1;

		prev = node;
		node = __vfs_get_parent(node);
	}

	assert(path_len >= ll);

	memmove(path, p, path_len - ll);

	if (node != prev) {
		return 1;
	}

	return 0;
}
*/