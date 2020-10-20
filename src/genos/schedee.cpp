#include <genos/schedee.h>
#include <genos/sched.h>
#include <errno.h>

#include <igris/dprint.h>

DLIST_HEAD(schedee_list);

uint16_t pid_counter = 0;
uint16_t gid_counter = 0;

uint16_t generate_new_pid()
{
	genos::schedee * sch;

	bool founded = false;
	do
	{
		pid_counter ++;
		if (pid_counter == 0) pid_counter++;

		founded = false;
		dlist_for_each_entry(sch, &schedee_list, schedee_list_lnk)
		{
			if (pid_counter == sch->pid)
			{
				founded = true;
				break;
			}
		}

	}
	while (founded);

	return pid_counter;
}

uint16_t generate_new_gid()
{
	genos::schedee * sch;

	bool founded = false;
	do
	{
		gid_counter ++;
		if (gid_counter == 0) gid_counter++;

		founded = false;
		dlist_for_each_entry(sch, &schedee_list, schedee_list_lnk)
		{
			if (gid_counter == sch->gid)
			{
				founded = true;
				break;
			}
		}

	}
	while (founded);

	return gid_counter;
}





#ifdef MVFS_INCLUDED

int schedee_get_free_fd(genos::schedee * sch)
{
	for (int i = 0; i < SCHEDEE_FDMAX; ++i)
	{
		if (sch->fds[i] == NULL)
			return i;
	}
	return -1;
}

int schedee_setfd(genos::schedee * sch, struct file * filp, int fd)
{
	if (sch->fds[fd] != NULL)
		return ENOENT;

	sch->fds[fd] = filp;
	return 0;
}

#endif //MVFS_INCLUDED

void schedee_notify_finalize(genos::schedee * sch)
{
	/*if (sch->parent != NULL
		&& sch->parent->sch_state == SCHEDEE_STATE_WAIT_SCHEDEE)
	{
		schedee_run(sch->parent);
	}*/

	assert(sch->parent);
	if (sch->parent)
		sch->parent->signal_handler(SIGCHLD);
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

void genos::schedee::signal_handler(int sig)
{
	if (sig == SIGCHLD)
	{
		if (sch_state == SCHEDEE_STATE_WAIT_SCHEDEE)
			start();
		return;
	}

	if (sig == SIGINT)
	{
		__schedee_final(this);
		return;
	}

	dprln("UNHSIG:", mnemo, sig);
}
