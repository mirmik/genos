#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <igris/sync/syslock.h>
#include <genos/signal.h>

DLIST_HEAD(schedee_list);

struct dlist_head runlist[SCHEDEE_PRIORITY_TOTAL];
DLIST_HEAD(unstoplist);
DLIST_HEAD(waitlist);
DLIST_HEAD(finallist);


uint16_t pid_counter = 0;

genos::schedee * __current_schedee = NULL;
genos::schedee * genos::current_schedee()
{
	return __current_schedee;
}

#ifdef GENOS_SCHEDEE_INTROSPECT
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
#endif

genos::schedee::schedee()
{
	// В дальнейшем эту провеку следует убрать, так как нод
	// должен отстыковываться от списка по завершению работы.
	ctrobj_init(&ctr, CTROBJ_SCHEDEE_LIST);
	this->prio = SCHEDEE_PRIORITY_TOTAL - 1;
	sch_state = SCHEDEE_STATE_STOP;
	syslock_counter_save = 0;
	parent = current_schedee();
	local_errno = 0;

#ifdef GENOS_SCHEDEE_INTROSPECT
	dlist_add(&schedee_list_lnk, &schedee_list);
	this->pid = generate_new_pid();
#endif
}

void genos::schedee_manager_init()
{

	for (int i = 0; i < SCHEDEE_PRIORITY_TOTAL; ++i)
		dlist_init(&runlist[i]);

	dlist_init(&waitlist);
	dlist_init(&unstoplist);
	dlist_init(&finallist);

	dlist_init(&schedee_list);
}

void genos::schedee_start(genos::schedee * sch)
{
	// TODO : Здесь должна быть какая-то защита от попытки оперировать таймером,
	//        находящимся в ожидании таймера.

	system_lock();
	sch->sch_state = SCHEDEE_STATE_RUN;
	sch->ctr.type = CTROBJ_SCHEDEE_LIST;

	dlist_move_tail(&sch->ctr.lnk, &unstoplist);

	system_unlock();
}

void genos::schedee_stop(genos::schedee * sch)
{
	// TODO : Здесь должна быть какая-то защита от попытки оперировать таймером,
	//        находящимся в ожидании таймера.

	system_lock();
	sch->sch_state = SCHEDEE_STATE_STOP;
	sch->ctr.type = CTROBJ_SCHEDEE_LIST;
	dlist_move_tail(&sch->ctr.lnk, &waitlist);
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
	sch->u.f.runned = 1;

//#if SCHEDEE_DEBUG_STRUCT
//	++sch->execcounter;
//#endif

	sch->execute();
}

void schedee_notify_finalize(genos::schedee * sch)
{
	if (sch->parent)
		sch->parent->signal_handler(SIGCHLD);
}


void genos::schedee_manager_step()
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

			if (sch->u.f.killed)
			{
				__schedee_final(sch);
				return;
			}

			dlist_move_tail(&sch->ctr.lnk, &runlist[priolvl]);
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
#ifdef GENOS_SCHEDEE_INTROSPECT
	dlist_del_init(&sch->schedee_list_lnk);
#endif
}

void genos::schedee::start() 
{
	genos::schedee_start(this);
}

void genos::schedee::stop() 
{
	genos::schedee_stop(this);
}
