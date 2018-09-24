/**
 	@file
 	Интерфейс вытеснения для драйверов и иных системных утилит.
 */

#ifndef GENOS_SCHED_WAIT_H
#define GENOS_SCHED_WAIT_H

#include <sys/cdefs.h>
#include <gxx/datastruct/dlist.h>

__BEGIN_DECLS

int wait_current_schedee(struct dlist_head * head);
int wait_current_schedee_priority(struct dlist_head * head);

int unwait_one(struct dlist_head * head);
int unwait_all(struct dlist_head * head);

__END_DECLS

#endif