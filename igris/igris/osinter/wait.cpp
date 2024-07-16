#include <igris/container/dlist.h>
#include <igris/datastruct/ring.h>
#include <igris/defs/io.h>
#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>
#include <igris/util/macro.h>

int waiter_unwait(igris::dlist_node *lnk, intptr_t future)
{
    waiter *w = mcast_out(lnk, waiter, lnk);
    w->future = future;
    assert(w->func != nullptr);
    w->func(reinterpret_cast<void *>(w->obj));
    return 0;
}

void unwait_one(igris::dlist_base *head, intptr_t future)
{
    igris::dlist_node *it;

    system_lock();

    if (head->empty())
    {
        system_unlock();
        return;
    }

    it = head->first_node();
    it->unlink();
    waiter_unwait(it, future);

    system_unlock();
}

void unwait_all(igris::dlist_base *head, intptr_t future)
{
    igris::dlist_node *it;

    system_lock();

    while (!head->empty())
    {
        it = head->first_node();
        it->unlink();
        waiter_unwait(it, future);
    }

    system_unlock();
}
