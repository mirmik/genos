#ifndef STM32_WATCHDOG_H
#define STM32_WATCHDOG_H

#include <sys/cdefs.h>
#include <stdint.h>

__BEGIN_DECLS

void stm32_watchdog_init(uint32_t presc, uint32_t counts);
void stm32_watchdog_enable_access();
void stm32_watchdog_start();
void stm32_watchdog_refresh();
void stm32_watchdog_set_prescaller(uint32_t presc);
void stm32_watchdog_set_reload_register(uint16_t val);
uint32_t stm32_watchdog_status_register();
void stm32_watchdog_wait_ready();

__END_DECLS

#endif