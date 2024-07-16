#ifndef STM32_ASM_STARTUP_H
#define STM32_ASM_STARTUP_H

extern unsigned char _estack;
#define IDLE_STACK &_estack;

#define RESET_STACK() {                 \
	__asm__("ldr sp, =_estack");        \
}


#endif