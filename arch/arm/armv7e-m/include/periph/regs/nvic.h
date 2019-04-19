#ifndef ARMV7EM_NVIC_REGS_H
#define ARMV7EM_NVIC_REGS_H

struct nvic_regs 
{
	volatile uint32_t ISER[8]; //0xE000E100- 0xE000E11F
	volatile uint32_t RESERVED0[24];
	volatile uint32_t ICER[8]; //0XE000E180- 0xE000E19F
	volatile uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8]; //0XE000E200- 0xE000E21F
	volatile uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8]; //0XE000E280- 0xE000E29F
	volatile uint32_t RESERVED3[24];
	volatile uint32_t IABR[8]; //0xE000E300- 0xE000E31F
	volatile uint32_t RESERVED5[56];
	volatile uint32_t IPR[60]; //0xE000E400- 0xE000E4EF
	volatile uint32_t RESERVED6[644];
	volatile uint32_t STIR; //0xE000EF00
};

#endif