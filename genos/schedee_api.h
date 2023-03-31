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
    void current_schedee_msleep_without_displace(unsigned int ms);
    void current_schedee_msleep_without_displace(unsigned int ms,
                                                 int64_t start);
    int current_schedee_displace();

    int wait_for_avail(unsigned int fd);

    void send_signal_to_group(int gid, int sig);
}

#endif
