#include <mvfs/fs/joke/joke.h>
#include <igris/util/bug.h>

#include <mvfs/trace.h>

const struct node_operations joke_node_ops;

struct joke_node * joke_node_create(const char * name, size_t nlen)
{
	DTRACE();
	struct joke_node * ret = (struct joke_node *) malloc(sizeof(struct joke_node));;
	node_init(&ret->node, name, nlen, &joke_node_ops);
	return ret;
}

struct joke_node * joke_node_create_as_child(const char * name, size_t nlen,
        struct node * parent
                                            )
{
	DTRACE();
	struct joke_node * ret = joke_node_create(name, nlen);
	node_add_child(&ret->node, parent);
	return ret;
}

void joke_dealloc(struct node * node) 
{
	free(node);
}

struct super_block * joke_get_sb(struct file_system_type * fs, int flags, const void * data)
{
	DTRACE();
	return &joke_sb.sb;
}

void joke_kill_sb(struct file_system_type * fs, struct super_block * sb)
{
	DTRACE();
	BUG();
}

const struct file_system_operations joke_fs_ops =
{
	.get_sb = joke_get_sb,
	.kill_sb = joke_kill_sb
};

static
int joke_mkdir(struct node * parent, const char * name, size_t nlen, int flags)
{
	DTRACE();
	struct joke_node * node = joke_node_create_as_child(name, nlen, parent);
	node->node.flag.directory = 1;
	node->node.sb = &joke_sb.sb;
	return 0;
}

static
int joke_rmdir(struct node * node)
{
	dlist_del(&node->lnk);
	joke_dealloc(node);
	return 0;
}

const struct node_operations joke_node_ops =
{
	.mkdir = joke_mkdir,
	.rmdir = joke_rmdir
};

struct joke_node joke_root = { NODE_INIT(joke_root.node, "/", &joke_sb.sb, &joke_node_ops, NODE_DIRECTORY) };
struct joke_fstype_class joke_fstype = { FILE_SYSTEM_INIT(joke_fstype.fs, "joke", &joke_fs_ops) };
struct joke_super_block joke_sb = { SUPER_BLOCK_INIT(joke_sb.sb, &joke_fstype.fs, &joke_root.node) };