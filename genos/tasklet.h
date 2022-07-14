#ifndef GENOS_CONDITION_MANAGER_H
#define GENOS_CONDITION_MANAGER_H

#include <igris/compiler.h>
#include <igris/datastruct/dlist.h>

typedef void (*tasklet_callback_t)(void *);

struct tasklet
{
    struct dlist_head lnk;
    tasklet_callback_t callback;
    void *privdata;
};

__BEGIN_DECLS

void tasklet_init(struct tasklet *tasklet,
                  tasklet_callback_t callback,
                  void *privdata);
void tasklet_deinit(struct tasklet *tasklet);

void tasklet_plan(struct tasklet *tasklet);
void tasklet_unplan(struct tasklet *tasklet);

void tasklet_exec(struct tasklet *tasklet);

void tasklet_manager_step();

__END_DECLS

#endif