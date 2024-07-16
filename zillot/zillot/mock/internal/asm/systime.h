#ifndef STM32_ASM_SYSTIME_H
#define STM32_ASM_SYSTIME_H

#include <stdint.h>

typedef int64_t systime_t;
typedef int64_t systime_difference_t;

extern uint32_t __systick_config;

#define systime_lomax() (0)
#define systime_lopart() (0)
#define systime_check_flag() (0)

#endif