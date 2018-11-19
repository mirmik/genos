/**
 	@file
 	Интерфейс вытеснения для драйверов и иных системных утилит.
 */

#ifndef GENOS_SCHED_WAIT_H
#define GENOS_SCHED_WAIT_H

#include <sys/cdefs.h>
#include <gxx/datastruct/dlist.h>

#define WAIT_PRIORITY 1

__BEGIN_DECLS

int wait_current_schedee(struct dlist_head * head, int priority);
int wait_flag_setted(struct dlist_head * head, int priority);

void unwait_one(struct dlist_head * head);
void unwait_all(struct dlist_head * head);

__END_DECLS

#endif