#ifndef STM32_PERIPH_IRQDEFS
#define STM32_PERIPH_IRQDEFS

#ifndef __ASSEMBLER__
#include <stm32xxxx.h>
#endif

#ifndef NR_IRQS
#define NR_IRQS 150
#endif

#ifndef NVIC_IRQNO
#define NVIC_IRQNO(x) (x)
#endif

#ifndef IRQTABLE_IRQNO
#define IRQTABLE_IRQNO(x) (x+16)
#endif

#define GENERATE_VECTORS (NR_IRQS-16)

#define STM32_IRQ_RESERVED       (0) /* Reserved vector (only used with CONFIG_DEBUG) */
                                     /* Vector  0: Reset stack pointer value */
                                     /* Vector  1: Reset (not handler as an IRQ) */
#define STM32_IRQ_NMI            (2) /* Vector  2: Non-Maskable Interrupt (NMI) */
#define STM32_IRQ_HARDFAULT      (3) /* Vector  3: Hard fault */
#define STM32_IRQ_MEMFAULT       (4) /* Vector  4: Memory management (MPU) */
#define STM32_IRQ_BUSFAULT       (5) /* Vector  5: Bus fault */
#define STM32_IRQ_USAGEFAULT     (6) /* Vector  6: Usage fault */
#define STM32_IRQ_SVCALL        (11) /* Vector 11: SVC call */
#define STM32_IRQ_DBGMONITOR    (12) /* Vector 12: Debug Monitor */
                                     /* Vector 13: Reserved */
#define STM32_IRQ_PENDSV        (14) /* Vector 14: Pendable system service request */
#define STM32_IRQ_SYSTICK       (15) /* Vector 15: System tick */

#endif