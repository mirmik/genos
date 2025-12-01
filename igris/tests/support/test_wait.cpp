#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>
#include <igris/syncxx/event.h>
#include <igris/util/macro.h>

#include <support/cooperative_threads_test.h>

namespace
{
    struct linux_waiter
    {
        waiter w = {};
        igris::event event = {};
    };

    void linux_unwait_handler(void *arg)
    {
        auto *waiter = static_cast<linux_waiter *>(arg);
        waiter->event.signal();
    }

    int linux_wait_current(igris::dlist_base *head, int priority, void **future)
    {
        linux_waiter waiter;
        waiter.w.func = linux_unwait_handler;
        waiter.w.obj = reinterpret_cast<uintptr_t>(&waiter);

        system_lock();

        if (priority)
            head->move_front(waiter.w.lnk);
        else
            head->move_back(waiter.w.lnk);
        system_unlock();

        waiter.event.wait();

        if (future)
            *future = (void *)waiter.w.future;
        return 0;
    }

    int linux_unwait(waiter *w)
    {
        auto *waiter = mcast_out(w, struct linux_waiter, w);
        waiter->event.signal();
        return 0;
    }
}

int wait_current_schedee(igris::dlist_base *head, int priority, void **future)
{
    if (igris::test::coop::in_worker())
        return igris::test::coop::detail::wait_current(head, priority, future);
    return linux_wait_current(head, priority, future);
}

int unwait_schedee_waiter(waiter *w)
{
    if (igris::test::coop::detail::owns_waiter(w))
        return igris::test::coop::detail::unwait(w);
    return linux_unwait(w);
}
