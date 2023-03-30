#ifndef GENOS_COOPERATIVE_SCHEDEE_H
#define GENOS_COOPERATIVE_SCHEDEE_H

#include <asm/context.h>
#include <cstring>
#include <genos/schedee.h>
#include <zillot/context.h>

namespace genos
{
    struct coop_schedee : public schedee
    {
        struct context cntxt;

        int (*task)(void *);
        void *arg;
        void *heap;
        size_t heapsize;

        int ret;

    public:
        coop_schedee(int (*task)(void *),
                     void *arg,
                     void *heap,
                     int heapsize,
                     void (*destructor)(schedee *sched) = nullptr);
        coop_schedee(void (*destructor)(schedee *sched) = nullptr)
            : schedee(destructor)
        {
        }

        void init(int (*task)(void *),
                  void *arg,
                  void *heap,
                  int heapsize,
                  void (*destructor)(schedee *sched) = nullptr);

        void reset_context(int (*task)(void *), void *arg);

        void execute() override;
        void finalize() override;
        int displace() override;

        void copy_stack_from(coop_schedee *csch)
        {
            uint8_t *src = (uint8_t *)csch->heap;
            uint8_t *dst = (uint8_t *)heap;
            size_t sz = csch->heapsize;
            memcpy(dst, src, sz);
            void *sp = (void *)context_get_sp(&csch->cntxt);
            ptrdiff_t diff = (uint8_t *)sp - (uint8_t *)csch->heap;
            context_set_sp(&cntxt, (void *)((uint8_t *)heap + diff));
        }

        int context_save(int value)
        {
            return ::context_save(&cntxt, value);
        }

        void set_context_variable(int value)
        {
            ::context_set_variable(&cntxt, value);
        }
    };

    coop_schedee *create_coop_schedee(int (*foo)(void *),
                                      void *arg,
                                      void *stack,
                                      size_t stacksize);
    coop_schedee *create_process(int (*foo)(void *),
                                 void *arg,
                                 void *stack,
                                 size_t stacksize);
}

#endif