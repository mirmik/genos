#ifndef GXX_DATASTRUCT_POOL_PAGED_POOL_H
#define GXX_DATASTRUCT_POOL_PAGED_POOL_H

#include <gxx/datastruct/pool/pool.h>

struct mempage_head
{
    struct slist_head lnk;
    void *zone;
    size_t size;
};

struct paged_pool_head
{
    struct pool_head pool;
    struct slist_head pages;
};

__BEGIN_DECLS

static inline void mempage_init(struct mempage_head *mempage, void *zone,
                                unsigned int size)
{
    mempage->zone = zone;
    mempage->size = size;
}

static inline void paged_pool_init(struct paged_pool_head *head,
                                   unsigned int elemsz)
{
    pool_init(&head->pool, elemsz);
    slist_init(&head->pages);
}

static inline void paged_pool_engage(struct paged_pool_head *head,
                                     struct mempage_head *page)
{
    slist_add_next(&page->lnk, &head->pages);
    pool_engage(&head->pool, page->zone, page->size);
}

// static inline void paged_pool_empty_pages(struct paged_pool_head* head, int*
// retarr) {
//}

__END_DECLS

#endif
