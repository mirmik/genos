#ifndef IGRIS_WAIT_H
#define IGRIS_WAIT_H

#include <igris/compiler.h>
#include <igris/container/dlist.h>
#include <igris/datastruct/ring.h>
#include <igris/osinter/ctrobj.h>

#define WAIT_PRIORITY 1

class waiter
{
public:
    igris::dlist_node lnk = {};
    int64_t future = 0;
    void (*func)(void *) = nullptr;
    uintptr_t obj = 0;

public:
    waiter() = default;
};

__BEGIN_DECLS

static inline void
waiter_delegate_init(waiter *w, void (*func)(void *), void *obj)
{
    w->func = func;
    w->obj = reinterpret_cast<uintptr_t>(obj);
}

int wait_current_schedee(igris::dlist_base *head, int priority, void **future);
int unwait_schedee_waiter(waiter *w);

int waiter_unwait(igris::dlist_node *lnk, intptr_t future);
void unwait_one(igris::dlist_base *head, intptr_t future);
void unwait_all(igris::dlist_base *head, intptr_t future);

__END_DECLS

#endif
