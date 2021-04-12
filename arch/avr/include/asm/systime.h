#ifndef AVR_ASM_SYSTIME_H
#define AVR_ASM_SYSTIME_H

//extern int __systick_config;

#define systime_lomax() 1 //__systick_config
//#define systime_lopart() (__systick_config - SysTick->VAL)
//#define systime_check_flag() (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)

#endif