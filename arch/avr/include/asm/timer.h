#ifndef GENOS_AVR_TIMER_H
#define GENOS_AVR_TIMER_H

#include <periph/regs/timer.h>
#include <sys/cdefs.h>
#include <inttypes.h>

struct timer_irqs {
	uint8_t ovf;
};

struct TimerDevice_8bit {
	struct timer_8bit_regs*	regs;
	struct timer_irqs	 		irqs;
	volatile uint8_t* 	timsk;
};

struct TimerDevice_16bit {
	struct timer_16bit_regs*	regs;
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

__END_DECLS

/*class TimerDevice_8bit : public TimerDevice<uint8_t> {
	timer_8bit_regs* regs;
	timer_irqs irqs;
	volatile reg_t* timsk;

public:

	TimerDevice_8bit(timer_8bit_regs* _regs, timer_irqs _irqs, volatile reg_t* _timsk) 
		: regs(_regs), irqs(_irqs), timsk(_timsk) {};

	uint8_t value();

	void start();
	void stop();

	void setLimit(uint8_t limit);
	void setDivider(uint16_t limit);

	void setHandlerOverflow(Kernel::IRQHandler handler);
	void interruptEnableOverflow(bool b);
};

class TimerDevice_16bit : public TimerDevice<uint16_t> {
	timer_16bit_regs* regs;
	timer_irqs irqs;
	volatile reg_t* timsk;

public:

	TimerDevice_16bit(timer_16bit_regs* _regs, timer_irqs _irqs, volatile reg_t* _timsk) 
		: regs(_regs), irqs(_irqs), timsk(_timsk) {};

	uint16_t value();

	void start();
	void stop();

	void setLimit(uint16_t limit);
	void setDivider(uint16_t divider);

	void setHandlerOverflow(Kernel::IRQHandler handler);
	void interruptEnableOverflow(bool b);
};
*/

#endif