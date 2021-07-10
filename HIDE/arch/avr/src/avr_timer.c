#include <periph/map.h>

#include <asm/avr_timer.h>
#include <igris/util/bits.h>
#include <igris/util/bug.h>

#include <assert.h>
#include <avr/io.h>

int16_t avr_timer_divider_code(uint16_t divider)
{
	switch (divider)
	{
	case 0 : return 0;
	case 1 : return 1;
	case 8 : return 2;
	case 64 : return 3;
	case 256 : return 4;
	}
	return -1;
}

void avr_timer8_divider(struct timer8_regs* regs, uint16_t divider)
{
	bits_assign(regs->tccr_b, 7, avr_timer_divider_code(divider));
}

void avr_timer16_divider(struct timer16_regs* regs, uint16_t divider)
{
	bits_assign(regs->tccr_b, 7, avr_timer_divider_code(divider));
}

uint8_t avr_timer8_value(struct timer8_regs* regs)
{
	return regs->tcnt;
}

uint16_t avr_timer16_value(struct timer16_regs* regs)
{
	return regs->tcnt;
}

static uint8_t avr_timer_no(uintptr_t regs)
{
	switch ((uintptr_t)regs)
	{
	case (uintptr_t)TIMER0: return 0;
	case (uintptr_t)TIMER1: return 1;
	case (uintptr_t)TIMER2: return 2;
#ifdef TIMER3
	case (uintptr_t)TIMER3: return 3;
#endif
#ifdef TIMER4
	case (uintptr_t)TIMER4: return 4;
#endif
	default: BUG();
	}
}

void avr_timer8_ovfirq_enable(struct timer8_regs* regs, uint8_t en)
{

	bits_assign(TIMSK[avr_timer_no((uintptr_t)regs)], 0x1, en);
}

void avr_timer16_ovfirq_enable(struct timer16_regs* regs, uint8_t en)
{
	bits_assign(TIMSK[avr_timer_no((uintptr_t)regs)], 0x1, en);
}

/*void tc_start(uint8_t channel, uint32_t divider) {
	assert(channel < TIMERS_TOTAL);
	assert( divider == TIMER_DIVIDER_1 ||
		divider == TIMER_DIVIDER_8 ||
		divider == TIMER_DIVIDER_64 ||
		divider == TIMER_DIVIDER_256 ||
		divider == TIMER_DIVIDER_1024 );

	if (bits_mask(1 << channel, TIMERS_8BIT_MAP)) {
		struct timer_8bit_regs* timer = (struct timer_8bit_regs*) timers_array[channel];
		bits_mask_assign(timer->tccr_b, 7, divider);
	}

	else if (bits_mask(1 << channel, TIMERS_16BIT_MAP)) {
		struct timer_16bit_regs* timer = (struct timer_16bit_regs*) timers_array[channel];
		bits_mask_assign(timer->tccr_b, 7, divider);
	}
}

uint8_t tc_total() {
	return TIMERS_TOTAL;
}

void tc_ovf_int_enable(uint8_t channel, void(*handler)(void*)) {
	assert(channel < TIMERS_TOTAL);
	switch(channel) {
		case 0: irq_handler_attach(ATMEGA_IRQ_TIM0_OVF, handler, 0); break;
#if TIMERS_TOTAL >= 2
		case 1: irq_handler_attach(ATMEGA_IRQ_TIM1_OVF, handler, 0); break;
#endif

#if TIMERS_TOTAL >= 3
		case 2: irq_handler_attach(ATMEGA_IRQ_TIM2_OVF, handler, 0); break;
#endif

#if TIMERS_TOTAL >= 4
		case 3: irq_handler_attach(ATMEGA_IRQ_TIM3_OVF, handler, 0); break;
#endif

#if TIMERS_TOTAL >= 5
		case 4: irq_handler_attach(ATMEGA_IRQ_TIM4_OVF, handler, 0); break;
#endif

		default: panic("tc_ovf_int_enable");
	}

	if (bits_mask(1 << channel, TIMERS_8BIT_MAP)) {
		struct timer_8bit_regs* timer = (struct timer_8bit_regs*) timers_array[channel];
		TIMSK[channel] |= 0x01;
	}

	else if (bits_mask(1 << channel, TIMERS_16BIT_MAP)) {
		struct timer_16bit_regs* timer = (struct timer_16bit_regs*) timers_array[channel];
		TIMSK[channel] |= 0x01;
	}
}*/