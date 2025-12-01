#ifndef IGRIS_DATASTRUCT_SPARSE_ARRAY_H
#define IGRIS_DATASTRUCT_SPARSE_ARRAY_H

#include <igris/compiler.h>
#include <igris/util/decltypeof.h>

struct sparse_array
{
    void *start;
    int stride;
};

#define SPARSE_ARRAY_INIT(strt, strd) {strt, strd};

#define SPARSE_ARRAY(name, strt, strd)                                         \
    struct sparse_array name = SPARSE_ARRAY_INIT(strt, strd);

__BEGIN_DECLS

static inline void sparse_array_init(struct sparse_array *array, void *start,
                                     int stride)
{
    array->start = start;
    array->stride = stride;
}

__END_DECLS

#define sparse_array_ptr(array, no, type)                                      \
    ((type *)(((char *)(array)->start) + (array)->stride * (no)))

#define sparse_array_ref(array, no, type)                                      \
    (*(type *)(((char *)(array)->start) + (array)->stride * (no)))

#define sparse_array_next(it, stride) ((decltypeof(it))((char *)it + (stride)))

#define sparse_array_for_each(it, array, size)                                 \
    for (it = (decltypeof(it))(array)->start;                                  \
         it != sparse_array_ptr((array), (size), decltypeof(*it));             \
         it = sparse_array_next(it, ((array)->stride)))

#endif
