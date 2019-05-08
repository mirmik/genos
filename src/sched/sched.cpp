#define NODTRACE 1

#include <sched/sched.h>
#include <igris/sync/syslock.h>

#include <igris/dtrace.h>
#include <igris/util/stub.h>
#include <util/delay.h>

#define PRIORITY_TOTAL 4

static struct dlist_head runlist[PRIORITY_TOTAL];
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

void schedee_run(struct schedee * sch)
{
	DTRACE();
	system_lock();
	//dprln("a"); 
	sch->state = SCHEDEE_STATE_RUN;
	dlist_move_tail(&sch->lnk, &runlist[sch->prio]);
	system_unlock();
}

void __schedee_wait_for(struct schedee * parent, struct schedee * child)
{
	DTRACE();
	system_lock();
	parent->state = SCHEDEE_STATE_WAIT_SCHEDEE;
	dlist_move_tail(&parent->lnk, &waitlist);
	system_unlock();
}

void schedee_wait(struct schedee * sch)
{
	DTRACE();
	system_lock();
	sch->state = SCHEDEE_STATE_WAIT;
	dlist_move_tail(&sch->lnk, &waitlist);
	system_unlock();
}

void schedee_final(struct schedee * sch)
{
	DTRACE();

	system_lock();
	sch->state = SCHEDEE_STATE_FINAL;
	dlist_move_tail(&sch->lnk, &finallist);
	system_unlock();
}

void schedee_stop(struct schedee * sch)
{
	DTRACE();
	system_lock();
	sch->state = SCHEDEE_STATE_STOP;
	dlist_del_init(&sch->lnk);
	system_unlock();
}

void __schedee_execute(struct schedee * sch)
{
	DTRACE();
	__current_schedee = sch;
	sch->flag.runned = 1;
	++sch->execcounter;
	system_unlock();
	sch->sch_op->execute(sch);
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

	system_lock();
	while (!dlist_empty(&finallist))
	{

		//dprln("finallist is not empty");
		
		if (!dlist_check(&finallist, 5)) 
		{
			dprln("Inconsistent final list. TODO");

			dprptrln(&finallist);
			dprptrln(finallist.next);
			dprptrln(finallist.next->next);
			dprptrln(finallist.next->next->next);

			struct dlist_head * it;
			int i = 0;
			dlist_for_each(it, &finallist) 
			{
				i++;
				dprptrln(it);

				if (i == 20) while(1);
			}

			while(1);
		}

		sch = dlist_first_entry(&finallist, struct schedee, lnk);

		dlist_del_init(&sch->lnk);
		schedee_notify_finalize(sch);
		
		sch->state = SCHEDEE_STATE_ZOMBIE;

		system_unlock();
		sch->sch_op->finalize(sch);

		system_lock();
	}
	system_unlock();

	//atomic_section_enter();
	//schedee_manager_debug_info();

	system_lock();
	for (int priolvl = 0; priolvl < PRIORITY_TOTAL; priolvl++)
	{
		if (!dlist_empty(&runlist[priolvl]))
		{
			sch = dlist_first_entry(&runlist[priolvl], struct schedee, lnk);

			//static int t = 0;

			//	t++;

			//if (t > 100)
			//if (dlist_size(&runlist[priolvl]) == 2) 
			//{
			//	dprln("start scheduling");
			//	dlist_debug_print(&runlist[priolvl]);
			//}

			dlist_move_tail(&sch->lnk, &runlist[priolvl]);

			//if (t > 5)
			//if (dlist_size(&runlist[priolvl]) == 2) 
			//{
			//	dlist_debug_print(&runlist[priolvl]);
			//}

			__schedee_execute(sch); //unlock here
			return;
		}
	}

	system_unlock();

	//Nobody to run
	return;
}

int __displace__()
{
	DTRACE();
	struct schedee * sch = current_schedee();

	if (sch->flag.can_displace == 0)
		return DISPLACE_ERROR;

	++sch->dispcounter;
	return sch->sch_op->displace(sch);
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
		dlist_for_each_entry(it, &runlist[priolvl], lnk) 
		{
			dpr("\t"); dprptrln(it);
		}
	}

	system_unlock();
}
