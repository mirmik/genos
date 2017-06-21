#ifndef KERNEL_TESTSCHEDEE_H
#define KERNEL_TESTSCHEDEE_H

#include "compiler.h"
#include "kernel/sched/schedee.h"

struct testschedee {
	struct schedee sch;
	const char* msg;
};

__BEGIN_DECLS

void testschedee_init(struct testschedee* tsch);
void debug_print_testschedee(struct testschedee* tsch);

__END_DECLS

#endif