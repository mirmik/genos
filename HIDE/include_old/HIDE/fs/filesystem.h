#ifndef GENOS_FILESYSTEM_H
#define GENOS_FILESYSTEM_H

#include <sys/cdefs.h>
#include "fs/struct/file_system.h"

__BEGIN_DECLS

void vfs_init();
int register_filesystem(struct file_system_type * fs);

__END_DECLS

#endif