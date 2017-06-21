#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include "debug/dprint.h"

//#include <hal/arch.h>
#include <hal/board.h>
#include <hal/irq.h>

#include "util/location.h"
#include "compiler.h"

#define panic(...) 									\
	do { 											\
		global_irqs_disable();						\
		debug_print("KERNEL PANIC:"); dln();		\
		debug_print_location(current_location()); 	\
		debug_print("message: ");					\
		debug_print(__VA_ARGS__);					\
		dln();										\
		board_shutdown(ARCH_SHUTDOWN_MODE_ABORT);	\
	} while (0)

#endif