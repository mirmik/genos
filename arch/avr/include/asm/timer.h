#ifndef GENOS_AVR_TIMER_H
#define GENOS_AVR_TIMER_H

#include <periph/regs/timer.h>
#include <sys/cdefs.h>
#include <inttypes.h>

struct timer_irqs {
	uint8_t ovf;
};

struct TimerDevice_8bit {
	struct timer8_regs*	regs;
	struct timer_irqs	 		irqs;
	volatile uint8_t* 	timsk;
};

struct TimerDevice_16bit {
	struct timer16_regs*	regs;
	struct timer_irqs	 		irqs;
	volatile uint8_t* 	timsk;
};

#if defined (CHIP_ATMEGA2560)
extern struct TimerDevice_8bit timer0;
extern struct TimerDevice_8bit timer2;
extern struct TimerDevice_16bit timer1;
extern struct TimerDevice_16bit timer3;
extern struct TimerDevice_16bit timer4;
extern struct TimerDevice_16bit timer5;
#elif defined (CHIP_ATMEGA328P)
extern struct TimerDevice_8bit timer0;
extern struct TimerDevice_8bit timer2;
extern struct TimerDevice_16bit timer1;
#else
#	error "unrecognized chip"
#endif

__BEGIN_DECLS

void tc_8bit_divider(struct TimerDevice_8bit* timer, uint16_t div);
void tc_8bit_interruptOverflowEnable(struct TimerDevice_8bit* timer, uint8_t en);

//void avr_timer8_set_divider(struct timer8_regs* regs, uint16_t div);
//void avr_timer16_set_divider(struct timer16_regs* regs, uint16_t div);

__END_DECLS

#endif