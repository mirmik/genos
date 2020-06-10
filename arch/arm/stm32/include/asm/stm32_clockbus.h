#ifndef STM32_CLOCKBUS_H
#define STM32_CLOCKBUS_H

#include <sys/cdefs.h>
#include <stdint.h>

#define CLOCKBUS_HSI 0
#define CLOCKBUS_HSE 1
#define CLOCKBUS_MSI 2
#define CLOCKBUS_PLL 3
#define CLOCKBUS_APB1 4
#define CLOCKBUS_APB2 5

extern uint32_t stm32_clockbus_freq[6];

__BEGIN_DECLS

void stm32_clockbus_hsi_mode();
void stm32_clockbus_hse_mode();

void stm32_clockbus_reevaluate();

void stm32_clockbus_systime_setup();

void stm32_clockbus_set_dividers(int a, int b);

__END_DECLS

#endif