#define NODTRACE 1

#include <genos/sched.h>

#include <asm/startup.h> // for RESET_STACK
#include <util/cpu_delay.h>
#include <hal/irq.h>

#include <igris/sync/syslock.h>
#include <igris/dtrace.h>
#include <igris/util/stub.h>

#define PRIORITY_TOTAL 4

struct dlist_head runlist[PRIORITY_TOTAL];
DLIST_HEAD(unstoplist);
DLIST_HEAD(waitlist);
DLIST_HEAD(finallist);
//struct dlist_head zombie_list;

struct schedee * __current_schedee = NULL;
struct schedee * current_schedee()
{
	return __current_schedee;
}

void schedee_manager_init()
{
	DTRACE();

	for (int i = 0; i < PRIORITY_TOTAL; ++i)
		dlist_init(&runlist[i]);
}

void __schedee_run(struct schedee * sch)
{
	DTRACE();
	dlist_move_tail(&sch->ctr.lnk, &runlist[sch->prio]);
}

void schedee_run(struct schedee * sch) 
{
	DTRACE();
	system_lock();
	sch->state = SCHEDEE_STATE_RUN;
	sch->ctr.type = CTROBJ_SCHEDEE_LIST;

	// detail: Если это объект отличный от schedee_list, благодаря бинарной совместимости перемещение
	// будет работать корректно. 
	dlist_move_tail(&sch->ctr.lnk, &unstoplist);

	system_unlock();
}

void __schedee_wait_for(struct schedee * parent, struct schedee * child)
{
	DTRACE();
	system_lock();
	parent->state = SCHEDEE_STATE_WAIT_SCHEDEE;
	dlist_move_tail(&parent->ctr.lnk, &waitlist);
	system_unlock();
}

void __schedee_final(struct schedee * sch)
{
	DTRACE();

	system_lock();
	sch->state = SCHEDEE_STATE_FINAL;
	dlist_move_tail(&sch->ctr.lnk, &finallist);
	system_unlock();
}

void schedee_stop(struct schedee * sch)
{
	DTRACE();
	system_lock();
	sch->state = SCHEDEE_STATE_STOP;
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();
}

void schedee_exit()
{
	struct schedee * sch;

	sch = current_schedee();
	__schedee_final(sch);

	__displace__();
}

void __schedee_execute(struct schedee * sch)
{
	DTRACE();
	__current_schedee = sch;
	sch->flag.runned = 1;

#if SCHEDEE_DEBUG_STRUCT
	++sch->execcounter;
#endif

	sch->execute();
}

int schedee_manager_total_planed() 
{
	int sum = 0;
	struct dlist_head* sch;
	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++)
	{
		dlist_for_each(sch, &runlist[priolvl]) { sum++; }
	}	
	return sum;
}

void schedee_manager_step()
{
	DTRACE();
	struct schedee* sch;

	// Отрабатываем логику завершения процессов.
	irqs_disable();
	while (!dlist_empty(&finallist))
	{
		sch = dlist_first_entry(&finallist, struct schedee, ctr.lnk);
		dlist_del_init(&sch->ctr.lnk);

		irqs_enable();

		schedee_notify_finalize(sch);
		sch->state = SCHEDEE_STATE_ZOMBIE;
		sch->finalize();

		irqs_disable();
	}
	irqs_enable();

	// Перемещаем процессы, объявленные запущенными в основные листы.
	irqs_disable();
	while (!dlist_empty(&unstoplist))
	{
		sch = dlist_first_entry(&unstoplist, struct schedee, ctr.lnk);
		__schedee_run(sch);
	}
	irqs_enable();
	

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
			__schedee_execute(sch);

			return;
		}
	}

	//Nobody to run
	return;
}

int __displace__()
{
	DTRACE();
	struct schedee * sch = current_schedee();

	if (sch->flag.can_displace == 0)
		return DISPLACE_ERROR;

#if SCHEDEE_DEBUG_STRUCT
	++sch->dispcounter;
#endif

	return sch->displace();
}

void scheduler_init() 
{
	schedee_manager_init();
}

void schedee_manager_debug_info() 
{
	system_lock();

	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++)
	{
		struct schedee * it;

		dprln("priolvl:", priolvl);
		dlist_for_each_entry(it, &runlist[priolvl], ctr.lnk) 
		{
			dpr("\t"); dprptrln(it);
		}
	}

	system_unlock();
}

void __context_displace_vector__()
{
	RESET_STACK();
	irqs_enable();
	syslock_reset();
	while (1) __schedule__();
}