#ifndef STM32_REGS_SYSCFG_H
#define STM32_REGS_SYSCFG_H

#include <asm/chip.h>

#ifdef CHIP_STM32F4XX
struct syscfg_regs
{
	volatile uint32_t MEMRMP;	//0x00
	volatile uint32_t PMC;		//0x04

	union
	{
		volatile uint32_t EXTICR[4];
		struct
		{
			volatile uint32_t EXTICR1;	//0x08
			volatile uint32_t EXTICR2;	//0x0C
			volatile uint32_t EXTICR3;	//0x10
			volatile uint32_t EXTICR4;	//0x14
		};
	};

	uint32_t RESERVED[2];		//0x18, 0x0C
	volatile uint32_t CMPCR;	//0x20
};

#define SYSCFG_EXTI_PORTA 0b0000
#define SYSCFG_EXTI_PORTB 0b0001
#define SYSCFG_EXTI_PORTC 0b0010
#define SYSCFG_EXTI_PORTD 0b0011
#define SYSCFG_EXTI_PORTE 0b0100
//101 - reserved
//110 - reserved
#define SYSCFG_EXTI_PORTH 0b0111

#endif

#endif