#ifndef GENOS_SCHEDEE_API_H
#define GENOS_SCHEDEE_API_H

#include <igris/compiler.h>
#include <genos/schedee.h>

__BEGIN_DECLS

void __schedee_final(genos::schedee * sch);
void current_schedee_exit();
int current_schedee_msleep(unsigned int ms);
int current_schedee_displace();

__END_DECLS

#endif
