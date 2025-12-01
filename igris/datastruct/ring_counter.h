#ifndef IGRIS_DATASTRUCT_RING_COUNTER_H
#define IGRIS_DATASTRUCT_RING_COUNTER_H

#include <igris/compiler.h>

struct ring_counter
{
    int counter;
    int size;
};

__BEGIN_DECLS

static inline void ring_counter_init(struct ring_counter *rc, int size)
{
    rc->counter = 0;
    rc->size = size;
}

static inline void ring_counter_fixup(struct ring_counter *rc)
{
    while (rc->counter >= rc->size)
        rc->counter -= rc->size;
}

static inline int ring_counter_fixup_pos(const struct ring_counter *rc, int pos)
{
    while (pos >= rc->size)
        pos -= rc->size;
    while (pos < 0)
        pos += rc->size;
    return pos;
}

static inline void ring_counter_set(struct ring_counter *rc, int val)
{
    rc->counter = val;
    ring_counter_fixup(rc);
}

static inline void ring_counter_increment(struct ring_counter *rc, int arg)
{
    rc->counter += arg;
    ring_counter_fixup(rc);
}

static inline int ring_counter_get(const struct ring_counter *rc)
{
    return rc->counter;
}

/// Вернуть номер, бывший на i итераций ранее.
static inline int ring_counter_prev(const struct ring_counter *rc, int i)
{
    int c = rc->counter - i;

    while (c < 0)
        c += rc->size;

    return c;
}

static inline int ring_counter_last(const struct ring_counter *rc, int no)
{
    return ring_counter_fixup_pos(rc, rc->counter - no);
}

__END_DECLS

#endif
