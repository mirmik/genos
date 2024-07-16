#ifndef GENOS_LOOKUP_H
#define GENOS_LOOKUP_H

#include <genos/resource/file_head.h>

namespace genos
{
    int mvfs_lookup(genos::file_head **filp, const char *path);
}

#endif