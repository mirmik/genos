#include <asm/irq.h>
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
    _execmon.start_session();
    irqs_enable();
    context_load(&cntxt);
}

int genos::coop_schedee::displace()
{
    _execmon.end_session();
    context_save_and_displace(&cntxt);
    return SCHEDEE_DISPLACE_REAL;
}

void genos::coop_schedee::finalize()
{
    if (dynamic_heap_flag())
    {
        free(heap);
        heap = nullptr;
        clear_dynamic_heap_flag();
    }
}

genos::coop_schedee::~coop_schedee()
{
    if (dynamic_heap_flag())
        free(heap);
}

genos::coop_schedee::coop_schedee(int (*task)(void *),
                                  void *arg,
                                  void *heapptr,
                                  int heapsize,
                                  void (*destructor)(schedee *))
    : schedee(destructor)
{
    assert(heapsize);

    this->heap = heapptr;
    this->heapsize = heapsize;
    if (this->heap == nullptr)
    {
        this->heap = malloc(this->heapsize);
        if (this->heap == nullptr)
        {
            abort();
        }
        this->set_dynamic_heap_flag();
    }

    this->task = task;
    this->arg = arg;
    this->set_has_context_flag();
    this->set_is_process_flag();

    context_init(&cntxt,
                 (uintptr_t)this->heap + heapsize - 1,
                 coop_schedee_starter,
                 (void *)this,
                 1);
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
        foo, arg, stack, stack_heap_size, +[](schedee *csch) { delete csch; });
    return csch;
}

void genos::coop_schedee::reset_context(int (*task)(void *), void *arg)
{
    igris::syslock_guard guard;
    this->task = task;
    this->arg = arg;
    context_init(&cntxt,
                 (uintptr_t)heap + heapsize - 1,
                 coop_schedee_starter,
                 (void *)this,
                 1);
}
