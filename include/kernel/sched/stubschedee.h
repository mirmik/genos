#ifndef GENOS_STUB_SCHEDEE_H
#define GENOS_STUB_SCHEDEE_H

#include <kernel/sched/schedee.h>
#include <kernel/panic.h>
#include <compiler.h>

#include <mem/sysalloc.h>

struct stub_schedee  {
	struct schedee sch;
};

__BEGIN_DECLS

struct stub_schedee * construct_stub_schedee();

__END_DECLS

#endif