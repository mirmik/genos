#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/schedee.h>
#include <igris/compiler.h>

namespace genos
{
    typedef void (*autom_schedee_func_t)(void *priv, int *state);

    class autom_schedee : public schedee
    {
        autom_schedee_func_t func;
        void *priv;
        int state;

    public:
        autom_schedee(void (*destructor)(schedee *sched) = nullptr);
        autom_schedee(autom_schedee_func_t foo,
                      void *privdata,
                      void (*destructor)(schedee *sched) = nullptr);

        autom_schedee(const autom_schedee &) = delete;
        autom_schedee &operator=(const autom_schedee &) = delete;

        void execute() override;
        void finalize() override;
        int displace() override;

        ~autom_schedee() override = default;
    };

    autom_schedee *create_autom_schedee(autom_schedee_func_t foo,
                                        void *privdata);

}

#endif