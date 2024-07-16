#ifndef IGRIS_DATASTRUCT_NAMED_TABLE_H
#define IGRIS_DATASTRUCT_NAMED_TABLE_H

#include <igris/compiler.h>
#include <cstddef>
#include <cstring>

struct nametbl_node
{
    const char *name;
    void *data;
};

struct nametbl2_node
{
    const char *name;
    void *data0;
    void *data1;
};

__BEGIN_DECLS

static inline struct nametbl_node *nametbl_find(struct nametbl_node *table,
                                                const char *name)
{
    for (struct nametbl_node *it = table; it->name != NULL; ++it)
    {
        if (strcmp(name, it->name) == 0)
            return it;
    }

    return NULL;
}

static inline struct nametbl2_node *nametbl2_find(struct nametbl2_node *table,
                                                  const char *name)
{
    for (struct nametbl2_node *it = table; it->name != NULL; ++it)
    {
        if (strcmp(name, it->name) == 0)
            return it;
    }

    return NULL;
}

__END_DECLS

#endif
