#ifndef STM32_ASM_SYSCFG_H
#define STM32_ASM_SYSCFG_H

#include <sys/cdefs.h>
#include <stm32xxxx.h>
#include <igris/util/bits.h>

#define SYSCFG_EXTI_PORTA 0b0000
#define SYSCFG_EXTI_PORTB 0b0001
#define SYSCFG_EXTI_PORTC 0b0010
#define SYSCFG_EXTI_PORTD 0b0011
#define SYSCFG_EXTI_PORTE 0b0100
//101 - reserved
//110 - reserved
#define SYSCFG_EXTI_PORTH 0b0111

__BEGIN_DECLS

static inline 
void stm32_syscfg_exti_map(uint8_t pinno, uint8_t portspec) 
{
	uint8_t regno = pinno / 4;
	uint8_t offno = pinno % 4;
	uint8_t offset = 4 * offno;

	bits_assign(SYSCFG->EXTICR[regno], 0b1111 << offset, portspec << offset);
}

__END_DECLS

#endif