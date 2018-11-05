#ifndef GENOS_CSHEDULER_H
#define GENOS_CSHEDULER_H

#include <stdint.h>
#include <gxx/util/bits.h>
#include <datastruct/dlist_head.h>
#include <genos/panic.h>

#include <kernel/sched/cschedee.h>
/*
void idle_func() {
//	printf("HelloWorld\r\n");
}*/

enum sched_flags_e {
	SCHED_READY = 1,
};

struct sched_t {
	dlist_head run_list;	//готовые процессы.
	dlist_head stop_list;	//остановленные процессы.
	dlist_head kill_list;	//процессы, готовые к уничтожению.

	//struct func_schedee_t idle; //idle процесс.
	struct schedee_t * curproc; //текущий процесс.

	uint8_t flags;
};

static inline void sched_toRunList(struct sched_t * sched, struct schedee_t * sch) { 
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK,SCHEDEE_STATE_RUN);
	dlist_move_next(&sch->lnk, &sched->run_list);
}

static inline void sched_toWaitList(struct sched_t * sched, struct schedee_t * sch) { 
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK,SCHEDEE_STATE_STOP); 
	dlist_move_next(&sch->lnk, &sched->stop_list);
}

static inline void sched_toKillList(struct sched_t * sched, struct schedee_t * sch) { 
	bits_mask_assign(sch->flags, SCHEDEE_STATE_MASK,SCHEDEE_STATE_KILL); 
	dlist_move_next(&sch->lnk, &sched->kill_list);
}

static inline void sched_init(struct sched_t * sched) {
	dlist_init(&sched->run_list);
	dlist_init(&sched->stop_list);
	dlist_init(&sched->kill_list);
	sched->flags = 0;

	//func_schedee_init(&sched->idle, do_nothing);
	//func_schedee_init(&sched->idle, idle_func);

	//sched_toRunList(sched, (struct schedee_t*) &sched->idle);
};

static inline void sched_unblock(struct sched_t * sched) {
	bits_set(sched->flags, SCHED_READY);
};

/** 
*/
static inline void sched_schedule(struct sched_t * sched) {
	//Если планировщик блокирован, panic
	if (!bits_mask(sched->flags, SCHED_READY)) //return;
		panic("SchedulerBlocked");

	//Уничтожить все единицы в списке смертников.
	while(!dlist_empty(&sched->kill_list)) {
		struct schedee_t * sch = dlist_first_entry(
			&sched->kill_list, struct schedee_t, lnk);
		dlist_del_init(&sch->lnk);
		sch->ops->destruct(sch);
	}

	events:
	sched->event_check();
	if 

	//Следующий цикл ищет самый приоритетный процесс,
	//попутно повышая приоритет каждого на единицу.
	int8_t max_prio = -1;
	struct schedee_t * sch = NULL;
	struct schedee_t * ptr;
	dlist_for_each_entry(ptr, &sched->run_list, lnk) {
		if (ptr->prio++ > max_prio) {
			sch = ptr;
			max_prio = ptr->prio;
		} 
	}

	//Вернуть приоритет процесса к изначальному.
	sch->prio = sch->static_prio;
	
	sch->ops->execute(sch);
}

#endif