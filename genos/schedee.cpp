#include <genos/schedee.h>
#include <igris/sync/syslock.h>
#include <genos/signal.h>

DLIST_HEAD(schedee_list);

struct dlist_head runlist[SCHEDEE_PRIORITY_TOTAL];
DLIST_HEAD(unstoplist);
DLIST_HEAD(waitlist);
DLIST_HEAD(finallist);


uint16_t pid_counter = 0;
uint16_t gid_counter = 0;

genos::schedee * __current_schedee = NULL;
genos::schedee * current_schedee()
{
	return __current_schedee;
}

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


genos::schedee::schedee(
    int prio,
    int flags)
{
#if SCHEDEE_DEBUG_STRUCT

	// В дальнейшем эту провеку следует убрать, так как нод
	// должен отстыковываться от списка по завершению работы.
	if (!dlist_in(&schedee_list_lnk, &schedee_list))
	{
		ctrobj_init(&ctr, CTROBJ_SCHEDEE_LIST);
		dlist_add(&schedee_list_lnk, &schedee_list);
	}

	else
	{
		dlist_del_init(&ctr.lnk);
	}

	this->pid = generate_new_pid();

	if (flags & SCHEDEE_USE_PARENT_GID)
		this->gid = current_schedee()->gid;
	else
		this->gid = generate_new_gid();

	dispcounter = 0;
	execcounter = 0;
#else
	ctrobj_init(&ctr, CTROBJ_SCHEDEE_LIST);
#endif

	this->prio = prio;
	this->sch_state = SCHEDEE_STATE_STOP;
	flags = 0;
	syslock_counter_save = 0;
	parent = current_schedee();
	local_errno = 0;
}

void schedee_manager_init()
{

	for (int i = 0; i < SCHEDEE_PRIORITY_TOTAL; ++i)
		dlist_init(&runlist[i]);

	dlist_init(&waitlist);
	dlist_init(&unstoplist);
	dlist_init(&finallist);

	dlist_init(&schedee_list);
}

void schedee_start(genos::schedee * sch)
{
	// TODO : Здесь должна быть какая-то защита от попытки оперировать таймером,
	//        находящимся в ожидании таймера.

	system_lock();
	sch->sch_state = SCHEDEE_STATE_RUN;
	sch->ctr.type = CTROBJ_SCHEDEE_LIST;

	dlist_move_tail(&sch->ctr.lnk, &unstoplist);

	system_unlock();
}



void __schedee_run(genos::schedee * sch)
{
	dlist_move_tail(&sch->ctr.lnk, &runlist[sch->prio]);
}

void __schedee_final(genos::schedee * sch)
{
	system_lock();
	sch->sch_state = SCHEDEE_STATE_FINAL;
	dlist_move_tail(&sch->ctr.lnk, &finallist);
	system_unlock();
}

void __schedee_execute(genos::schedee * sch)
{
	__current_schedee = sch;
	sch->flag.runned = 1;

#if SCHEDEE_DEBUG_STRUCT
	++sch->execcounter;
#endif

	sch->execute();
}

void schedee_notify_finalize(genos::schedee * sch)
{
	if (sch->parent)
		sch->parent->signal_handler(SIGCHLD);
}


void schedee_manager_step()
{
	genos::schedee* sch;

	// Отрабатываем логику завершения процессов.
	system_lock();
	while (!dlist_empty(&finallist))
	{
		sch = dlist_first_entry(&finallist, genos::schedee, ctr.lnk);
		dlist_del_init(&sch->ctr.lnk);

		system_unlock();

		sch->sch_state = SCHEDEE_STATE_ZOMBIE;
		sch->finalize();

		schedee_notify_finalize(sch);

		system_lock();
	}
	system_unlock();

	// Перемещаем процессы, объявленные запущенными в основные листы.
	system_lock();
	while (!dlist_empty(&unstoplist))
	{
		sch = dlist_first_entry(&unstoplist, genos::schedee, ctr.lnk);
		__schedee_run(sch);
	}
	system_unlock();


	for (int priolvl = 0; priolvl < SCHEDEE_PRIORITY_TOTAL; priolvl++)
	{
		if (!dlist_empty(&runlist[priolvl]))
		{
			sch = dlist_first_entry(&runlist[priolvl], genos::schedee, ctr.lnk);

			if (sch->flag.killed)
			{
				__schedee_final(sch);
				return;
			}

			dlist_move_tail(&sch->ctr.lnk, &runlist[priolvl]);

#if SCHED_DEBUG
			dprln("execute:", (uintptr_t)sch, sch->mnemo);
#endif

			__schedee_execute(sch);

			return;
		}
	}

	//Nobody to run
	return;
}


void schedee_deinit(genos::schedee * sch) 
{
	dlist_del_init(&sch->ctr.lnk);
#if SCHEDEE_DEBUG_STRUCT
	dlist_del_init(&sch->schedee_list_lnk);
#endif
}