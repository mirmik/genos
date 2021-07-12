#include <genos/resource/namespace.h>

DLIST_HEAD(namespace_list);

void namespace_init(struct namespace_head * head, const char * path, 
	const struct namespace_operations * ops) 
{
	head->path = path;
	head->ns_ops = ops;
	dlist_add(&head->lnk_namespace_list, &namespace_list);
}