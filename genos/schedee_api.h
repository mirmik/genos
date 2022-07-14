#ifndef GENOS_SCHEDEE_API_H
#define GENOS_SCHEDEE_API_H

#include <genos/schedee.h>
#include <igris/compiler.h>

namespace genos
{
    void __schedee_final(genos::schedee *sch);
    void current_schedee_exit();
    int current_schedee_msleep(unsigned int ms);
    int current_schedee_msleep(unsigned int ms, int64_t start);
    int current_schedee_displace();
}

#endif
