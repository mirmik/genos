#ifndef __ASSEMBLER__

#include <periph/map.h>
#define NVIC_IRQNO(x) (x)
#define IRQTABLE_IRQNO(x) (x+16)

#else

#include <periph/core_irqdefs.h>

#if defined(CHIP_STM32L412XX)
#	define GENERATE_VECTORS 83
#elif defined(CHIP_STM32L432XX)
#	define GENERATE_VECTORS 83
#else
#	error "move NR_IRQS here"
#endif

#endif