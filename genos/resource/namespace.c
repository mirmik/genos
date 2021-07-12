#include <genos/resource/namespace.h>
#include <string.h>

DLIST_HEAD(namespace_list);

void namespace_init(struct namespace_head * head, const char * path,
                    const struct namespace_operations * ops)
{
	head->path = path;
	head->ns_ops = ops;
	dlist_add(&head->lnk_namespace_list, &namespace_list);
}

void namespace_deinit(struct namespace_head * head)
{
	dlist_del(&head->lnk_namespace_list);
}

struct namespace_head * namespace_lookup(const char * path)
{
	struct namespace_head * ns;
	struct namespace_head * maxns = NULL;
	int maxlen = 0;

	int pathlen = strlen(path);
	dlist_for_each_entry(ns, &namespace_list, lnk_namespace_list)
	{
		const char * aptr = path;
		const char * bptr = ns->path;

		int ns_pathlen = strlen(ns->path);
		if (pathlen < ns_pathlen) 
			continue;

		int i = 0;
		while (aptr[i] == bptr[i] && i < ns_pathlen) 
		{
			++i;
		}

		if (maxlen < i)
		{
			maxlen = i;
			maxns = ns;
		}
	}

	return maxns;
}