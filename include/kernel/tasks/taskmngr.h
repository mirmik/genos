#ifndef TASKLET_MANAGER_H
#define TASKLET_MANAGER_H

#include <compiler.h>
#include <kernel/tasks/tasklet.h>

__BEGIN_DECLS

void tasklet_plan(struct tasklet* tsk);

void tasklet_unplan(struct tasklet* tsk);

void tasklet_execute(struct tasklet* tsk);

__END_DECLS

#endif