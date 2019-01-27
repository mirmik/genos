#include <drivers/timer/avr_timer.h>

namespace periph {
#if defined (CHIP_ATMEGA2560)
	genos::avr::timer8 	timer0(TIMER0, {ATMEGA_IRQ_TIM0_OVF}, &TIMSK[0]);
	genos::avr::timer16 timer1(TIMER1, {ATMEGA_IRQ_TIM1_OVF}, &TIMSK[1]);
	genos::avr::timer8 	timer2(TIMER2, {ATMEGA_IRQ_TIM2_OVF}, &TIMSK[2]);
	genos::avr::timer16 timer3(TIMER3, {ATMEGA_IRQ_TIM3_OVF}, &TIMSK[3]);
	genos::avr::timer16 timer4(TIMER4, {ATMEGA_IRQ_TIM4_OVF}, &TIMSK[4]);
	genos::avr::timer16 timer5(TIMER5, {ATMEGA_IRQ_TIM5_OVF}, &TIMSK[5]);
#elif defined (CHIP_ATMEGA328P)
	genos::avr::timer8 	timer0(TIMER0, {ATMEGA_IRQ_TIM0_OVF}, &TIMSK[0]);
	genos::avr::timer16 timer1(TIMER1, {ATMEGA_IRQ_TIM1_OVF}, &TIMSK[1]);
	genos::avr::timer8 	timer2(TIMER2, {ATMEGA_IRQ_TIM2_OVF}, &TIMSK[2]);
#endif
}