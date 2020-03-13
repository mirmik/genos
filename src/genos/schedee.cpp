#include <genos/schedee.h>
#include <genos/sched.h>
#include <errno.h>

#include <igris/dprint.h>

DLIST_HEAD(schedee_list);

#ifdef MVFS_INCLUDED

int schedee_get_free_fd(genos::schedee * sch) {
	for(int i = 0; i < SCHEDEE_FDMAX; ++i) {
		if (sch->fds[i] == NULL)
			return i;
	}
	return -1;
}

int schedee_setfd(genos::schedee * sch, struct file * filp, int fd) {
	if (sch->fds[fd] != NULL) 
		return ENOENT;

	sch->fds[fd] = filp;
	return 0;
}

#endif //MVFS_INCLUDED

void schedee_notify_finalize(genos::schedee * sch) {
	if (sch->parent != NULL 
		&& sch->parent->sch_state == SCHEDEE_STATE_WAIT_SCHEDEE) 
	{
		schedee_run(sch->parent); 
	}
}

#if SCHEDEE_DEBUG_STRUCT
void schedee_list_debug_info() 
{
	struct dlist_head * it;
	dlist_for_each(it, &schedee_list) 
	{
		genos::schedee * sch = mcast_out(it, genos::schedee, schedee_list_lnk);
		dprptr(sch);
		dpr(" execcounter:");
		dpr(sch->execcounter);
		dpr(" dispcounter:");
		dpr(sch->dispcounter);
		dpr(" prio:");
		dpr(sch->prio);
		dpr(" sch_state");
		dpr(sch->sch_state);
		dprln("");
	}
}
#endif

void genos::schedee::start() 
{
	schedee_run(this);
}

void genos::schedee::stop() 
{
	schedee_stop(this);
}