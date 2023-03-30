#include <cstring>
#include <genos/resource/lookup.h>
#include <genos/resource/namespace.h>

genos::namespace_manager *namespace_lookup(const char *path,
                                           const char **internal_path)
{
    genos::namespace_manager *maxns = NULL;
    int maxlen = 0;

    int pathlen = strlen(path);
    for (auto &ns : genos::namespace_list)
    {
        const char *aptr = path;
        const char *bptr = ns.path();

        int ns_pathlen = strlen(ns.path());
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
            maxns = &ns;
        }
    }

    if (internal_path)
    {
        *internal_path = path + maxlen;

        while ((**internal_path) == '/')
        {
            ++(*internal_path);
        }
    }

    return maxns;
}

int genos::mvfs_lookup(struct file_head **filp, const char *path)
{
    const char *internal_path = path;
    namespace_manager *ns = namespace_lookup(path, &internal_path);
    if (ns == NULL)
        return -1;
    return ns->lookup(filp, internal_path);
}
