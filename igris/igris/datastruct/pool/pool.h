#ifndef GXX_DATASTRUCT_POOL_POOL_H
#define GXX_DATASTRUCT_POOL_POOL_H

#include <gxx/datastruct/slist.h>

struct pool_head
{
    struct slist_head free_blocks;
    size_t elemsz;
};

__BEGIN_DECLS

static inline void pool_init(struct pool_head *head, size_t elemsz)
{
    slist_init(&head->free_blocks);
    head->elemsz = elemsz;
}

static inline void pool_engage(struct pool_head *pool, void *zone, size_t size)
{
    char *stop = (char *)zone + size;
    char *it = (char *)zone;

    while (it < stop)
    {
        slist_add_next((slist_head *)it, &pool->free_blocks);
        it += pool->elemsz;
    }
}

static inline void *pool_alloc(struct pool_head *head)
{
    return (void *)slist_pop_first(&head->free_blocks);
}

static inline void pool_free(struct pool_head *head, void *ptr)
{
    slist_add_next((slist_head *)ptr, &head->free_blocks);
}

__END_DECLS

#endif
