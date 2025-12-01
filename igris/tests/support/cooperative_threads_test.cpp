#include <support/cooperative_threads_test.h>

#include <errno.h>
#include <ucontext.h>

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include <igris/container/dlist.h>
#include <igris/osinter/wait.h>

namespace igris::test::coop
{
    struct thread
    {
        ucontext_t context;
        igris::dlist_node ready_link = {};
        waiter wait = {};
        std::unique_ptr<uint8_t[]> stack;
        size_t stack_size = 0;
        thread_fn entry = nullptr;
        void *arg = nullptr;
        bool waiting = false;
        bool finished = false;

        thread(thread_fn fn, void *a, size_t stack_sz)
            : stack(new uint8_t[stack_sz]), stack_size(stack_sz), entry(fn), arg(a)
        {
            wait.obj = reinterpret_cast<uintptr_t>(this);
        }
    };

    static std::vector<std::unique_ptr<thread>> threads;
    static igris::dlist<thread, &thread::ready_link> ready_queue;
    static thread *current_thread = nullptr;
    static ucontext_t scheduler_context;
    static bool scheduler_running = false;

    static void wake_handler(void *arg)
    {
        auto *thr = reinterpret_cast<thread *>(arg);

        if (!thr->waiting)
            return;

        thr->waiting = false;
        ready_queue.move_back(*thr);
    }

    static void thread_trampoline(uintptr_t arg)
    {
        auto *thr = reinterpret_cast<thread *>(arg);
        thr->entry(thr->arg);
        thr->finished = true;
        swapcontext(&thr->context, &scheduler_context);
    }

    void reset()
    {
        ready_queue.clear();
        threads.clear();
        current_thread = nullptr;
        scheduler_running = false;
    }

    void spawn(thread_fn fn, void *arg, size_t stack_size)
    {
        auto thr = std::make_unique<thread>(fn, arg, stack_size);
        getcontext(&thr->context);
        thr->context.uc_stack.ss_sp = thr->stack.get();
        thr->context.uc_stack.ss_size = thr->stack_size;
        thr->context.uc_link = &scheduler_context;
        makecontext(&thr->context, (void (*)())thread_trampoline, 1,
                    reinterpret_cast<uintptr_t>(thr.get()));
        ready_queue.move_back(*thr);
        threads.push_back(std::move(thr));
    }

    void run()
    {
        scheduler_running = true;
        while (!ready_queue.empty())
        {
            thread &thr = ready_queue.front();
            ready_queue.pop_front();
            current_thread = &thr;
            swapcontext(&scheduler_context, &thr.context);

            if (!thr.finished && !thr.waiting)
            {
                ready_queue.move_back(thr);
            }
        }

        current_thread = nullptr;
        scheduler_running = false;
    }

    void yield()
    {
        if (current_thread == nullptr)
            return;

        swapcontext(&current_thread->context, &scheduler_context);
    }

    bool scheduler_active()
    {
        return scheduler_running;
    }

    bool in_worker()
    {
        return current_thread != nullptr;
    }

    namespace detail
    {
        int wait_current(igris::dlist_base *head, int priority, void **future)
        {
            thread *thr = current_thread;
            assert(thr != nullptr);

            thr->wait.func = wake_handler;
            thr->waiting = true;

            if (priority)
                head->move_front(thr->wait.lnk);
            else
                head->move_back(thr->wait.lnk);

            swapcontext(&thr->context, &scheduler_context);

            if (future)
                *future = reinterpret_cast<void *>(thr->wait.future);

            return 0;
        }

        bool owns_waiter(const waiter *w)
        {
            if (w == nullptr)
                return false;
            auto *thr = reinterpret_cast<const thread *>(w->obj);
            return thr != nullptr;
        }

        int unwait(waiter *w)
        {
            if (!owns_waiter(w))
                return -EINVAL;

            auto *thr = reinterpret_cast<thread *>(w->obj);
            wake_handler(reinterpret_cast<void *>(thr));
            return 0;
        }
    }
}
