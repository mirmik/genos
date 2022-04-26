#ifndef GENOS_MVFS_H
#define GENOS_MVFS_H

#include <igris/compiler.h>
#include <genos/resource/namespace.h>

__BEGIN_DECLS

int mvfs_lookup(struct file_head ** filp, const char * path);
int mvfs_open(const char * path, int mode, int flags);

__END_DECLS

#endif