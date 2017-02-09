#ifndef ASSERT_H_
#define ASSERT_H_

#include "debug/dprint.h"
#include "util/location.h"
#include "kernel/panic.h"
#include "board.h"

#define assert(condition) \
	  ({__assert(condition, #condition, "");})

#define assertf(condition, ...) \
	  ({__assert(condition, #condition, __VA_ARGS__);})

#define static_assert(cond) \
	typedef int __assertation_helper[(cond) ? 1 : -1]

#define __assert(cond,strcond,message)				\
	do { if ((cond) == 0) {							\
		global_irq_disable();						\
		debug_print("ASSERTATION:"); dln();			\
		debug_print_location(current_location()); 	\
		debug_print("condition: ");					\
		debug_print(strcond); dln();				\
		board_shutdown(ARCH_SHUTDOWN_MODE_ABORT);	\
		}											\
	} while(0)				

#endif /* ASSERT_H_ */
