#ifndef GXX_DATASTRUCT_HTABLE_H
#define GXX_DATASTRUCT_HTABLE_H

/**
    @file datastruct/htable.h
    @brief Здесь задается структура хэштаблицы на основе slist.
*/

#include <igris/datastruct/slist.h>

struct stable_head
{
    struct slist_head *arr;
    unsigned int len;
};

__BEGIN_DECLS

static inline void stable_init(struct stable_head *stbl,
                               struct slist_head *sarray, size_t size)
{
    for (struct slist_head *it = sarray, *eit = sarray + size; it != eit; ++it)
    {
        slist_init(it);
    }
}

static inline void stable_add_hash(struct stable_head *stbl, unsigned int hash,
                                   struct slist_head *node)
{
    unsigned int bucket = hash % stbl->len;
    slist_add(node, &stbl->arr[bucket]);
}

static inline struct slist_head *
stable_get_hash_bucket(struct stable_head *stbl, unsigned int hash)
{
    unsigned int bucket = hash % stbl->len;
    return &stbl->arr[bucket];
}

__END_DECLS

#define stable_for_each(pos, head)                                             \
    __stable_for_each(pos, MACRO_GUARD(bucket), head)

#define __stable_for_each(pos, bucket, head)                                   \
    for (int bucket = 0; bucket < tbl->len; ++bucket)                          \
    slist_for_each(pos, head->arr[bucket])

#define stable_for_each_entry(pos, head, member)                               \
    __stable_for_each_entry(pos, MACRO_GUARD(bucket), head, member)

#define __stable_for_each_entry(pos, bucket, head, member)                     \
    for (int bucket = 0; bucket < tbl->len; ++bucket)                          \
    slist_for_each_entry(pos, head->arr[bucket], member)

#endif
