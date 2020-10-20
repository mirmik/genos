#define NODTRACE 1

#include <genos/sched.h>

#if !__NOARCH
#include <asm/startup.h> // for RESET_STACK
#else 
#define RESET_STACK() 
#endif

#include <util/cpu_delay.h>
#include <hal/irq.h>

#include <igris/sync/syslock.h>
#include <igris/dtrace.h>
#include <igris/defs/schedee.h>
//#include <igris/util/stub.h>

#define PRIORITY_TOTAL 4

struct dlist_head runlist[PRIORITY_TOTAL];
DLIST_HEAD(unstoplist);
DLIST_HEAD(waitlist);
DLIST_HEAD(finallist);
//struct dlist_head zombie_list;

genos::schedee * __current_schedee = NULL;
genos::schedee * current_schedee()
{
	return __current_schedee;
}

void schedee_manager_init()
{

	for (int i = 0; i < PRIORITY_TOTAL; ++i)
		dlist_init(&runlist[i]);
}

void __schedee_run(genos::schedee * sch)
{
	dlist_move_tail(&sch->ctr.lnk, &runlist[sch->prio]);
}

void schedee_run(genos::schedee * sch)
{
	//dprln("SCHEDEE_RUN");
	system_lock();
	sch->sch_state = SCHEDEE_STATE_RUN;
	sch->ctr.type = CTROBJ_SCHEDEE_LIST;

	// detail: Если это объект отличный от schedee_list, благодаря бинарной
	// совместимости перемещение будет работать корректно.
	dlist_move_tail(&sch->ctr.lnk, &unstoplist);

	system_unlock();
}

void __schedee_wait_for(genos::schedee * parent, genos::schedee * child)
{
	system_lock();
	parent->sch_state = SCHEDEE_STATE_WAIT_SCHEDEE;
	dlist_move_tail(&parent->ctr.lnk, &waitlist);
	system_unlock();
}

void __schedee_final(genos::schedee * sch)
{
	system_lock();
	sch->sch_state = SCHEDEE_STATE_FINAL;
	dlist_move_tail(&sch->ctr.lnk, &finallist);
	system_unlock();
}

void schedee_stop(genos::schedee * sch)
{
	system_lock();
	sch->sch_state = SCHEDEE_STATE_STOP;
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();
}

void schedee_exit()
{
	genos::schedee * sch;

	sch = current_schedee();
	__schedee_final(sch);

	__displace__();
}

void __schedee_execute(genos::schedee * sch)
{
	__current_schedee = sch;
	sch->flag.runned = 1;

	syslock_counter_set(sch->syslock_counter_save);
	if (sch->syslock_counter_save != 0)
		irqs_disable();
	else
		irqs_enable();

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
	dlist_for_each(sch, &unstoplist) { sum++; }
	
	return sum;
}

const char * schedee_state_string(uint8_t state) 
{
	switch (state) 
	{
		case SCHEDEE_STATE_RUN: return "RUN";
		case SCHEDEE_STATE_STOP: return "STOP";
		case SCHEDEE_STATE_WAIT: return "WAIT";
		case SCHEDEE_STATE_WAIT_SCHEDEE: return "WAITSCH";
		case SCHEDEE_STATE_FINAL: return "FINAL";
		case SCHEDEE_STATE_ZOMBIE: return "ZOMB";
		default: return "";
	}
}

static void ps_command()
{
#if SCHEDEE_DEBUG_STRUCT
	genos::schedee* sch;
	dlist_for_each_entry(sch, &schedee_list, schedee_list_lnk)
	{
		if (sch->mnemo)
			dpr(sch->mnemo);
		else
			dpr("unnamed");

		dpr(" pid:"); dpr(sch->pid);
		dpr(" gid:"); dpr(sch->gid);
		dpr(" exec:"); dpr(sch->execcounter);
		dpr(" disp:"); dpr(sch->dispcounter);
		dpr(" state:"); dpr(schedee_state_string(sch->sch_state));

		dprln();
	}
#endif
}

void schedee_manager_step()
{
	genos::schedee* sch;

	// Отрабатываем логику завершения процессов.
	irqs_disable();
	while (!dlist_empty(&finallist))
	{
		sch = dlist_first_entry(&finallist, genos::schedee, ctr.lnk);
		dlist_del_init(&sch->ctr.lnk);

		irqs_enable();

		sch->sch_state = SCHEDEE_STATE_ZOMBIE;
		sch->common_finalize();

		schedee_notify_finalize(sch);

		irqs_disable();
	}
	irqs_enable();

	// Перемещаем процессы, объявленные запущенными в основные листы.
	irqs_disable();
	while (!dlist_empty(&unstoplist))
	{
		sch = dlist_first_entry(&unstoplist, genos::schedee, ctr.lnk);
		__schedee_run(sch);
	}
	irqs_enable();


	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++)
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
			__schedee_execute(sch);

			return;
		}
	}

	//Nobody to run
	return;
}

int __displace__()
{
	genos::schedee * sch = current_schedee();

	if (sch->flag.can_displace == 0)
		return SCHEDEE_DISPLACE_ERROR;

	sch->syslock_counter_save = syslock_counter();

#if SCHEDEE_DEBUG_STRUCT
	++sch->dispcounter;
#endif

	//irqs_disable();
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
		genos::schedee * it;

		dprln("priolvl:", priolvl);
		dlist_for_each_entry(it, &runlist[priolvl], ctr.lnk)
		{
			dpr("\t"); dprptrln(it);
		}
	}

	system_unlock();
}

__attribute__((weak))
void __context_displace_vector__()
{
	RESET_STACK();
	irqs_enable();
	syslock_reset();
	while (1) __schedule__();
}

static genos::schedee* get_schedee_by_mnemo(const char* mnemo)
{
	genos::schedee* sch;
	dlist_for_each_entry(sch, &schedee_list, schedee_list_lnk)
	{
		if (sch->mnemo && strcmp(sch->mnemo, mnemo) == 0)
			return sch;
	}
	return nullptr;
}

static int procctr(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("usage: procctr CMD=run|stop MNEMO\r\n");
		return 0;
	}

	genos::schedee * sch;
	if ((sch = get_schedee_by_mnemo(argv[2])) == nullptr)
	{
		printf("unknown schedee\r\n");
		return 0;
	}

	if (strcmp(argv[1], "start") == 0)
	{
		schedee_run(sch);
	}

	else if (strcmp(argv[1], "stop") == 0)
	{
		schedee_stop(sch);
	}

	return 0;
}

igris::console_command schedee_manager_utilities[] =
{
	{"ps", ps_command},
	{"procctr", procctr},
	{nullptr, (void*)nullptr, nullptr}
};