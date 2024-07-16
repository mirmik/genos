#include <zillot/armv7e-m/arm_nvic.h>
#include <asm/irqdefs.h>

void nvic_enable_irq(uint32_t irqno) 
{
	irqno = NVIC_IRQNO(irqno);
	uint32_t regno = irqno >> 5;
	uint32_t bitno = irqno & 0x1F;
	NVIC->ISER[regno] |= 1 << bitno;
}
	
void nvic_set_priority(int irqno, int32_t prio) 
{
	irqno = NVIC_IRQNO(irqno);
	NVIC->IP[irqno] = prio;
}
