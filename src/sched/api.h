#ifndef GENOS_SCHEDEE_API_H
#define GENOS_SCHEDEE_API_H

#include <sched/sched.h>

__BEGIN_DECLS

extern void schedee_exit();
extern int schedee_sleep_for_milliseconds(int ms); 

static inline unsigned int msleep(unsigned int milliseconds) {
	return schedee_sleep_for_milliseconds(milliseconds);
}

__END_DECLS

#endif