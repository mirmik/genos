#include <genos/tasklet.h>

DLIST_HEAD(tasklets_list);

void tasklet_init(
	struct tasklet * tasklet,
	tasklet_callback_t callback,
	void * privdata) 
{
	tasklet -> callback = callback;
	tasklet -> privdata = privdata;
	dlist_init(&tasklet->lnk);
}

void tasklet_plan(struct tasklet * tasklet) 
{
	dlist_move(&tasklet->lnk, &tasklets_list);
}

void tasklet_unplan(struct tasklet * tasklet) 
{
	dlist_del_init(&tasklet->lnk);
}

void tasklet_deinit(struct tasklet * tasklet) 
{
	tasklet_unplan(tasklet);
}

void tasklet_exec(struct tasklet * tasklet) 
{
	tasklet->callback(tasklet->privdata);
}

void tasklet_manager_step() 
{
	struct tasklet *it, *n;

	dlist_for_each_entry_safe(it, n, &tasklets_list, lnk) 
	{
		tasklet_exec(it);
	}
}