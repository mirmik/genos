#include <cstring>
#include <genos/resource/lookup.h>
#include <genos/resource/namespace.h>

genos::namespace_manager *namespace_lookup(const char *path,
                                           const char **internal_path)
{
    genos::namespace_manager *maxns = NULL;
    size_t maxlen = 0;

    size_t pathlen = strlen(path);
    for (auto &ns : genos::namespace_list)
    {
        const char *ns_path = ns.path();
        size_t ns_pathlen = strlen(ns_path);
        if (pathlen < ns_pathlen)
            continue;

        if (strncmp(path, ns_path, ns_pathlen) != 0)
            continue;

        bool ns_ends_with_separator =
            ns_pathlen != 0 && ns_path[ns_pathlen - 1] == '/';
        char path_boundary = path[ns_pathlen];
        if (!ns_ends_with_separator && path_boundary != '\0' &&
            path_boundary != '/')
            continue;

        if (maxlen >= ns_pathlen)
            continue;

        maxlen = ns_pathlen;
        maxns = &ns;
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
