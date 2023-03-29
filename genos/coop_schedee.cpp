#include <genos/coop_schedee.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <igris/defs/schedee.h>
#include <igris/dprint.h>
#include <igris/util/bug.h>
#include <stdlib.h>

static void coop_schedee_starter(void *priv)
{
    genos::coop_schedee *csch = (genos::coop_schedee *)priv;
    auto task = csch->task;
    auto arg = csch->arg;
    int ret = task(arg);

    // if process is a child after fork (or something like ops)
    // then csch is not a current schedee.
    genos::coop_schedee *csch2 =
        (genos::coop_schedee *)genos::current_schedee();
    csch2->ret = ret;
    genos::current_schedee_exit();
}

void genos::coop_schedee::execute()
{
    context_load(&cntxt);
}

int genos::coop_schedee::displace()
{
    context_save_and_displace(&cntxt);
    return SCHEDEE_DISPLACE_REAL;
}

void genos::coop_schedee::finalize()
{
    if (u.f.dynamic_heap)
        free(heap);
}

genos::coop_schedee::coop_schedee(int (*task)(void *),
                                  void *arg,
                                  void *heap,
                                  int heapsize,
                                  void (*destructor)(schedee *))
{
    init(task, arg, heap, heapsize, destructor);
}

void genos::coop_schedee::init(int (*task)(void *),
                               void *arg,
                               void *heap,
                               int heapsize,
                               void (*destructor)(schedee *))
{
    assert(heapsize);

    set_destructor(destructor);

    if (heap == nullptr)
    {
        heap = malloc(this->heapsize);
        u.f.dynamic_heap = 1;
    }

    context_init(&cntxt,
                 (uintptr_t)heap + heapsize - 1,
                 coop_schedee_starter,
                 (void *)this,
                 1);

    this->heap = heap;
    this->heapsize = heapsize;
    this->task = task;
    this->arg = arg;
    this->u.f.has_context = 1;
    this->u.f.can_displace = 1;
    this->u.f.is_process = 1;
}

genos::coop_schedee *genos::create_coop_schedee(int (*foo)(void *),
                                                void *arg,
                                                void *stack,
                                                size_t stacksize)
{
    return genos::create_process(foo, arg, stack, stacksize);
}

genos::coop_schedee *genos::create_process(int (*foo)(void *),
                                           void *arg,
                                           void *stack,
                                           size_t stack_heap_size)

{
    genos::coop_schedee *csch = new genos::coop_schedee(
        foo,
        arg,
        stack,
        stack_heap_size,
        +[](schedee *)
        {
            auto *csch = (genos::coop_schedee *)current_schedee();
            delete csch;
        });
    return csch;
}

void genos::coop_schedee::reset_context(int (*task)(void *), void *arg)
{
    this->task = task;
    this->arg = arg;
    context_init(&cntxt,
                 (uintptr_t)heap + heapsize - 1,
                 coop_schedee_starter,
                 (void *)this,
                 1);
}