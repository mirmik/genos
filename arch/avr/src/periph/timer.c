#include <periph/regs/timer.h>
#include <arch/timer.h>
#include <periph/irqdefs.h>

#include <gxx/util/bits.h>

#include <assert.h>
#include <avr/io.h>

#if defined (CHIP_ATMEGA2560)
struct TimerDevice_8bit timer0 = {TIMER0, {ATMEGA_IRQ_TIM0_OVF}, &TIMSK[0]};
struct TimerDevice_8bit timer2 = {TIMER2, {ATMEGA_IRQ_TIM1_OVF}, &TIMSK[1]};
struct TimerDevice_16bit timer1 = {TIMER1, {ATMEGA_IRQ_TIM2_OVF}, &TIMSK[2]};
struct TimerDevice_16bit timer3 = {TIMER3, {ATMEGA_IRQ_TIM3_OVF}, &TIMSK[3]};
struct TimerDevice_16bit timer4 = {TIMER4, {ATMEGA_IRQ_TIM4_OVF}, &TIMSK[4]};
struct TimerDevice_16bit timer5 = {TIMER5, {ATMEGA_IRQ_TIM5_OVF}, &TIMSK[5]};
#elif defined (CHIP_ATMEGA328P)
struct TimerDevice_8bit timer0 = {TIMER0, {ATMEGA_IRQ_TIM0_OVF}, &TIMSK[0]};
struct TimerDevice_8bit timer2 = {TIMER2, {ATMEGA_IRQ_TIM1_OVF}, &TIMSK[1]};
struct TimerDevice_16bit timer1 = {TIMER1, {ATMEGA_IRQ_TIM2_OVF}, &TIMSK[2]};
#else
#	error "unrecognized chip"
#endif

uint8_t tc_divider_code(uint16_t divider) {
	switch (divider) {
		case 0 : return 0;
		case 1 : return 1;
		case 8 : return 2;
		case 64 : return 3;
		case 256 : return 4;
		//default: panic("wrong_divider");
	}
	return -1;
}

void tc_8bit_divider(struct TimerDevice_8bit* timer, uint16_t divider) {	
	bits_mask_assign(timer->regs->tccr_b, 7, tc_divider_code(divider)); 
}

void tc_16bit_divider(struct TimerDevice_16bit* timer, uint16_t divider) {
	bits_mask_assign(timer->regs->tccr_b, 7, tc_divider_code(divider)); 
}

uint8_t tc_8bit_value(struct TimerDevice_8bit* timer) {
	return timer->regs->tcnt;
}

uint16_t tc_16bit_value(struct TimerDevice_16bit* timer) {
	return timer->regs->tcnt;
}
/*
void TimerDevice_8bit::start() {};

void TimerDevice_16bit::start() {};

void TimerDevice_8bit::stop() {
	bits_mask_assign(regs->tccr_b, 7, 0); 
}

void TimerDevice_16bit::stop() {
	bits_mask_assign(regs->tccr_b, 7, 0); 
}

void TimerDevice_8bit::setLimit(uint8_t limit) {
	panic("ns");
}

void TimerDevice_16bit::setLimit(uint16_t limit) {
	panic("ns");
}
*/

void tc_8bit_interruptOverflowEnable(struct TimerDevice_8bit* timer, uint8_t en) {
	bits_mask_assign(*timer->timsk, 0x1, en);	
}

void tc_16bit_interruptOverflowEnable(struct TimerDevice_16bit* timer, uint8_t en){
	bits_mask_assign(*timer->timsk, 0x1, en);	
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