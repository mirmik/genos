#ifndef ASSERT_H_
#define ASSERT_H_

#include <genos/compiler.h>
#include <gxx/debug/dprint.h>
#include <gxx/util/location.h>
#include "genos/panic.h"
#include "hal/board.h"

#define assert(condition) \
	  ({__assert(condition, #condition, "");})

#define assertf(condition, ...) \
	  ({__assert(condition, #condition, __VA_ARGS__);})

#define __assert(cond,strcond,message)				\
	do { if ((cond) == 0) {							\
		global_irqs_disable();						\
		debug_print("ASSERTATION:"); dln();			\
		debug_print_location(current_location()); 	\
		debug_print("condition: ");					\
		debug_print(strcond); dln();				\
		board_shutdown(ARCH_SHUTDOWN_MODE_ABORT);	\
		}											\
	} while(0)				

#ifndef __cplusplus
#	define static_assert(a,b) _Static_assert(a,b)
#endif

#endif /* ASSERT_H_ */
