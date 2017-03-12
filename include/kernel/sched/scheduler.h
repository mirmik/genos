#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

#include <kernel/sched/schedee.h>
#include <stdbool.h>

#ifndef PRIORITY_TOTAL
#	define PRIORITY_TOTAL 6
#endif
__BEGIN_DECLS

struct schedee* planned_schedee();
void set_planned_schedee(struct schedee* sch);

void genos_idle();

void scheduler_free(void*);

void schedee_set_state_wait(struct schedee *sch, uint8_t state);
void schedee_set_state_run(struct schedee *sch);
void schedee_set_state_final(struct schedee *sch);
void schedee_set_state_zombie(struct schedee *sch);

bool schedee_state_is(struct schedee *sch, uint8_t state);

void set_final_deallocate(struct schedee *sch, bool en);
void set_final_release(struct schedee *sch, bool en);

bool state_is_run(struct schedee *sch);
bool state_is_wait(struct schedee *sch);
bool state_is_zombie(struct schedee *sch);
bool is_final_deallocated(struct schedee *sch);
bool is_final_release(struct schedee *sch);

void finalize_schedee(struct schedee* sch);

void __schedee_plan__();
void __schedule__();
void __displace__();

void kernel_stub_schedee_init();
void scheduler_init();

__END_DECLS

//#define set_state_run(sch) schedee_state_set(sch, SCHEDEE_STATE_RUN)
//#define set_state_wait(sch) schedee_state_set(sch, SCHEDEE_STATE_WAIT)
//#define set_state_final(sch) schedee_state_set(sch, SCHEDEE_STATE_FINAL)
//#define set_state_zombie(sch) schedee_state_set(sch, SCHEDEE_STATE_ZOMBIE)

#endif