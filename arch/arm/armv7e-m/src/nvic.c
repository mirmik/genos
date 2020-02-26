#include <asm/nvic.h>

#include <periph/regs/nvic.h>
#include <periph/irqdefs.h>
#include <periph/map.h>

#include <igris/dprint.h>

void dprint_dump_nvic()
{
	dprln("NVIC_DUMP:");
	dpr("\tISER: "); dprptr((void*)&NVIC->ISER[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tICER: "); dprptr((void*)&NVIC->ICER[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ICER[i]); dln();
	}

	dpr("\tISPR: "); dprptr((void*)&NVIC->ISPR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISPR[i]); dln();
	}

	dpr("\tICPR: "); dprptr((void*)&NVIC->ICPR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->ISER[i]); dln();
	}

	dpr("\tIABR: "); dprptr((void*)&NVIC->IABR[0]); dln();

	for (int i = 0; i < 8; ++i)
	{
		dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->IABR[i]); dln();
	}

	dpr("\tIP:  "); dprptr((void*)&NVIC->IP[0]); dln();

	//for (int i = 0; i < 8; ++i)
	//{
	//	dpr("\t\t"); dpr(i); dpr(": "); dprhex(NVIC->IP[i]); dln();
	//}

	dpr("\tSTIR: "); dprptr((void*)&NVIC->STIR); dln();
}

void nvic_enable_irq(uint32_t irqno) 
{
	irqno = NVIC_IRQNO(irqno);

	uint32_t regno = irqno >> 5;
	uint32_t bitno = irqno & 0x1F;

	assert(regno < 8);

	NVIC->ISER[regno] |= 1 << bitno;
};
