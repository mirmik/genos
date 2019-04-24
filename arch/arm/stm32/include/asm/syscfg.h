#ifndef STM32_ASM_SYSCFG_H
#define STM32_ASM_SYSCFG_H

#include <sys/cdefs.h>
#include <periph/map.h>

#include <igris/util/bits.h>

__BEGIN_DECLS

static inline 
void syscfg_exti_map(uint8_t pinno, uint8_t portspec) 
{
	uint8_t regno = pinno / 4;
	uint8_t offno = pinno % 4;
	uint8_t offset = 4 * offno;

	bits_assign(SYSCFG->EXTICR[regno], 0b1111 << offset, portspec << offset);
}

__END_DECLS

#endif