#ifndef GENOS_MVFS_H
#define GENOS_MVFS_H

#include <igris/compiler.h>
#include <genos/resource/namespace.h>

__BEGIN_DECLS

int mvfs_lookup(struct file_head ** filp, const char * path) 
{
	int sts;
	const char * internal_path;
	struct namespace_head * ns = namespace_lookup(path, &internal_path);

	if (ns == NULL)
		return -1;

	sts = ns->ns_ops->lookup(ns, filp, internal_path);
	return sts;
}

__END_DECLS

#endif