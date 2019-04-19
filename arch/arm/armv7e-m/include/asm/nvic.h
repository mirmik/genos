#ifndef ARMV7E_M_ASM_NVIC_H
#define ARMV7E_M_ASM_NVIC_H

#include <periph/map.h>
#include <assert.h>

static inline
void dprint_dump_nvic()
{
	dprln("NVIC_DUMP:");
	dpr("\tISER: "); dprptr(&NVIC->ISER[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tICER: "); dprptr(&NVIC->ICER[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tISPR: "); dprptr(&NVIC->ISPR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tICPR: "); dprptr(&NVIC->ICPR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tIABR: "); dprptr(&NVIC->IABR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tIPR:  "); dprptr(&NVIC->IPR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tSTIR: "); dprptr(&NVIC->STIR); dln();
}

static inline 
void nvic_enable_irq(uint32_t irqno) 
{
	uint32_t regno = irqno / 32;
	uint32_t bitno = irqno % 32;

	assert(regno < 8);

	NVIC->ISER[regno] |= 1 << bitno;
};

#endif