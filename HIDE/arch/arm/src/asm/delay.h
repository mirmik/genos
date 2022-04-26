#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>

extern float stm32_ticks_per_us_div_3;

static inline void init_delays(uint32_t cpufreqs) 
{
    stm32_ticks_per_us_div_3 = (float)cpufreqs / 1000000 / 3;
}

#define STM32_DELAY_US_MULT stm32_ticks_per_us_div_3

static inline void delay_us(uint32_t us) {
    us *= STM32_DELAY_US_MULT;

    /* fudge for function call overhead  */
    //us--;
    __asm__ volatile("   mov r0, %[us]          \n\t"
                 "1: subs r0, #1            \n\t"
                 "   bhi 1b                 \n\t"
                 :
                 : [us] "r" (us)
                 : "r0");
}

static inline void delay_ns100(uint32_t us) {
    us *= STM32_DELAY_US_MULT;
	us /= 10;

    /* fudge for function call overhead  */
    //us--;
    __asm__ volatile("   mov r0, %[us]          \n\t"
                 "1: subs r0, #1            \n\t"
                 "   bhi 1b                 \n\t"
                 :
                 : [us] "r" (us)
                 : "r0");
}
#endif