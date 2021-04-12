#ifndef NUCLEO_BOARDS_DEFS_H
#define NUCLEO_BOARDS_DEFS_H

#define STM32_FREQMODE_HSI 0
#define STM32_FREQMODE_HSE 1
#define STM32_FREQMODE_PLL_HIGH 2
#define STM32_FREQMODE_PLL_LOW 3

#include <sys/cdefs.h>

__BEGIN_DECLS

void board_init(int freqmode = STM32_FREQMODE_HSI);
void blink_test_cpudelay(int cpudelay);

__END_DECLS

#endif