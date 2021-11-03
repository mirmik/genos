#include <genos/resource/mvfs.h>
#include <genos/resource/openres.h>
#include <genos/resource/file_head.h>
#include <genos/schedee.h>

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

int mvfs_open(const char * path, int mode, int flags) 
{
	(void) mode;

	int sts;
	int resindex;
	struct schedee * sch;
	struct openres * res;

	struct file_head * filp;

	sts = mvfs_lookup(&filp, path);
	if (sts) 
		return sts;

	if (!file_head_is_open(filp)) 
	{
		sts = filp->f_ops->on_open(filp);
		if (sts) 
			return sts;
	}

	sch = current_schedee();
	resindex = schedee_get_free_openres(sch, &res);

	res->file = filp;
	filp->flags = flags; // TODO: Как должны выставляться флаги?

	return resindex;
}