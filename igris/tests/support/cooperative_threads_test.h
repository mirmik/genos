#ifndef IGRIS_TEST_COOPERATIVE_THREADS_H
#define IGRIS_TEST_COOPERATIVE_THREADS_H

// NOTE: Этот хелпер существует только в тестах. Он подменяет pthread-like
// потоки на крошечный кооперативный планировщик на базе ucontext.

#include <cstddef>

namespace igris
{
    class dlist_base;
}

class waiter;

namespace igris::test::coop
{
    using thread_fn = void (*)(void *);

    void reset();
    void
    spawn(thread_fn fn, void *arg = nullptr, size_t stack_size = 64 * 1024);
    void run();
    void yield();
    bool scheduler_active();
    bool in_worker();

    namespace detail
    {
        int wait_current(igris::dlist_base *head, int priority, void **future);
        bool owns_waiter(const waiter *w);
        int unwait(waiter *w);
    }
}

#endif
