#ifndef DATASTRUCT_LINHTABLE_H
#define DATASTRUCT_LINHTABLE_H

#include <gxx/datastruct/hlist.h>

struct htable_head
{
    struct hlist_head *arr;
    unsigned int len;
};

__BEGIN_DECLS

static inline void htable_init(struct htable_head *stbl,
                               struct hlist_head *sarray, size_t size)
{
    for (struct hlist_head *it = sarray, *eit = sarray + size; it != eit; ++it)
    {
        hlist_head_init(it);
    }
}

static inline void htable_add(struct htable_head *stbl, unsigned int hash,
                              struct hlist_node *node)
{
    unsigned int bucket = hash % stbl->len;
    hlist_add_next(node, &stbl->arr[bucket].first);
}

static inline struct hlist_head *htable_get_bucket(struct htable_head *stbl,
                                                   unsigned int hash)
{
    unsigned int bucket = hash % stbl->len;
    return &stbl->arr[bucket];
}

__END_DECLS

#define htable_for_each(pos, head)                                             \
    __htable_for_each(pos, MACRO_GUARD(bucket), head)

#define __htable_for_each(pos, bucket, head)                                   \
    for (int bucket = 0; bucket < tbl->len; ++bucket)                          \
    hlist_for_each(pos, head->arr[bucket])

#define htable_for_each_entry(pos, head, member)                               \
    __htable_for_each_entry(pos, MACRO_GUARD(bucket), head, member)

#define __htable_for_each_entry(pos, bucket, head, member)                     \
    for (int bucket = 0; bucket < tbl->len; ++bucket)                          \
    hlist_for_each_entry(pos, head->arr[bucket], member)

__END_DECLS

#endif
