
#ifndef __SYSTEM_STM32F4XX_H
#define __SYSTEM_STM32F4XX_H

#include "sys/cdefs.h"

__BEGIN_DECLS

extern uint32_t SystemCoreClock;

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

__END_DECLS

#endif /*__SYSTEM_STM32F4XX_H */
