#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

#include <genos/sched/schedee.h>

#include <sys/cdefs.h>
#include <gxx/datastruct/dlist_head.h>

#define PRIORITY_TOTAL 3

typedef struct schedee_manager_s {
	struct dlist_head run_list[PRIORITY_TOTAL];
	struct dlist_head wait_list;
	struct dlist_head final_list;
	struct dlist_head zombie_list;

	schedee_t* cursch;
} schedee_manager_t;

__BEGIN_DECLS

void schedee_manager_init();
void schedee_manager_set_state(schedee_t* sch, uint8_t state);
void schedee_manager_schedule();

//pid_t current_schedee_pid();
schedee_t* current_schedee();

//Операция вытеснения процесса. Отрабатывает по разному в зависимости от schedee.
void __displace__();

__END_DECLS

#endif