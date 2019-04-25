#include <mvfs/node.h>
#include <mvfs/mvfs.h>
#include <mvfs/file.h>
#include <igris/dprint.h>
#include <mvfs/trace.h>

static inline void __vfs_dpr_node_tree(struct node * d, const int t)
{
	struct node * it;

	int tt = t;

	while (tt--) dpr("  "); dprln(d->name);

	dlist_for_each_entry(it, &d->childs, lnk)
	{
		__vfs_dpr_node_tree(it, t + 1);
	}
}

void vfs_dpr_node_tree(struct node * d)
{
	__vfs_dpr_node_tree(d ? d : vfs_get_pwd(), 0);
}

void node_init(struct node * node, const char * name, size_t nlen,
               const struct node_operations * n_ops)
{
	DTRACE();
	int len;

	len = nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX;

	strncpy(node->name, name, len);
	node->name[len] = '\0';
	node->flags = 0;
	node->n_ops = n_ops;

	dlist_init(&node->childs);
}

void node_add_child(struct node * node, struct node * parent)
{
	DTRACE();
	dlist_add_tail(&node->lnk, &parent->childs);
	node->parent = parent;
}

int vfs_open_node(struct node * i, struct file ** filpp)
{
	DTRACE();
	int sts;
	struct file * filp;

	filp = vfs_file_alloc();
	filp->node = i;

	if (i->n_ops->open == NULL)
		return ENOTSUP;

	sts = i->n_ops->open(i, filp);

	if (sts)
	{
		vfs_file_dealloc(filp);
		return sts;
	}

	*filpp = filp;

	return 0;
}