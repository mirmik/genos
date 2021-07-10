#include <genos/schedee.h>
#include <igris/sync/syslock.h>
#include <genos/signal.h>

DLIST_HEAD(schedee_list);

#define PRIORITY_TOTAL 4

struct dlist_head runlist[PRIORITY_TOTAL];
DLIST_HEAD(unstoplist);
DLIST_HEAD(waitlist);
DLIST_HEAD(finallist);


uint16_t pid_counter = 0;
uint16_t gid_counter = 0;

struct schedee * __current_schedee = NULL;
struct schedee * current_schedee()
{
	return __current_schedee;
}

uint16_t generate_new_pid()
{
	struct schedee * sch;

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
	struct schedee * sch;

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


void schedee_init(
    struct schedee* sch,
    int prio,
    int flags,
    const struct schedee_operations * ops)
{

#if SCHEDEE_DEBUG_STRUCT

	// В дальнейшем эту провеку следует убрать, так как нод
	// должен отстыковываться от списка по завершению работы.
	if (!dlist_in(&sch->schedee_list_lnk, &schedee_list))
	{
		ctrobj_init(&sch->ctr, CTROBJ_SCHEDEE_LIST);
		dlist_add(&sch->schedee_list_lnk, &schedee_list);
	}

	else
	{
		dlist_del_init(&sch->ctr.lnk);
	}

	sch->pid = generate_new_pid();

	if (flags & SCHEDEE_USE_PARENT_GID)
		sch->gid = current_schedee()->gid;
	else
		sch->gid = generate_new_gid();

	sch->dispcounter = 0;
	sch->execcounter = 0;
#else
	ctrobj_init(&sch->ctr, CTROBJ_SCHEDEE_LIST);
#endif

	sch->ops = ops;
	sch->prio = prio;
	sch->sch_state = SCHEDEE_STATE_STOP;
	sch->flags = 0;
	sch->syslock_counter_save = 0;

	sch->parent = current_schedee();

	sch->local_errno = 0;
}

void schedee_manager_init()
{

	for (int i = 0; i < PRIORITY_TOTAL; ++i)
		dlist_init(&runlist[i]);
}

void schedee_start(struct schedee * sch)
{
	// TODO : Здесь должна быть какая-то защита от попытки оперировать таймером,
	//        находящимся в ожидании таймера.

	system_lock();
	sch->sch_state = SCHEDEE_STATE_RUN;
	sch->ctr.type = CTROBJ_SCHEDEE_LIST;

	dlist_move_tail(&sch->ctr.lnk, &unstoplist);

	system_unlock();
}



void __schedee_run(struct schedee * sch)
{
	dlist_move_tail(&sch->ctr.lnk, &runlist[sch->prio]);
}

void __schedee_final(struct schedee * sch)
{
	system_lock();
	sch->sch_state = SCHEDEE_STATE_FINAL;
	dlist_move_tail(&sch->ctr.lnk, &finallist);
	system_unlock();
}

void __schedee_execute(struct schedee * sch)
{
	__current_schedee = sch;
	sch->flag.runned = 1;

#if SCHEDEE_DEBUG_STRUCT
	++sch->execcounter;
#endif

	sch->ops->execute(sch);
}

void schedee_notify_finalize(struct schedee * sch)
{
	if (sch->parent)
		sch->parent->signal_handler(SIGCHLD);
}


void schedee_manager_step()
{
	struct schedee* sch;

	// Отрабатываем логику завершения процессов.
	system_lock();
	while (!dlist_empty(&finallist))
	{
		sch = dlist_first_entry(&finallist, struct schedee, ctr.lnk);
		dlist_del_init(&sch->ctr.lnk);

		system_unlock();

		sch->sch_state = SCHEDEE_STATE_ZOMBIE;
		sch->ops->finalize(sch);

		schedee_notify_finalize(sch);

		system_lock();
	}
	system_unlock();

	// Перемещаем процессы, объявленные запущенными в основные листы.
	system_lock();
	while (!dlist_empty(&unstoplist))
	{
		sch = dlist_first_entry(&unstoplist, struct schedee, ctr.lnk);
		__schedee_run(sch);
	}
	system_unlock();


	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++)
	{
		if (!dlist_empty(&runlist[priolvl]))
		{
			sch = dlist_first_entry(&runlist[priolvl], struct schedee, ctr.lnk);

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
