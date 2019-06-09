#ifndef ARDUINO_PWM_H
#define ARDUINO_PWM_H

#include <drivers/timer/avr_timer.h>
#include <drivers/pwmservo/avr_pwmservo.h>

#ifdef BOARD_ARDUINO_MEGA
constexpr const struct genos::avr::pwmregs ARDUINO_PWM[] = 
{
	{ &genos::avr::timer3, AVR_TIMER_OCRB }, // 2
	{ &genos::avr::timer3, AVR_TIMER_OCRC }, // 3
	{ &genos::avr::timer0, AVR_TIMER_OCRB }, // 4
	{ &genos::avr::timer3, AVR_TIMER_OCRA }, // 5
	{ &genos::avr::timer4, AVR_TIMER_OCRA }, // 6
	{ &genos::avr::timer4, AVR_TIMER_OCRB }, // 7
	{ &genos::avr::timer4, AVR_TIMER_OCRC }, // 8
	{ &genos::avr::timer2, AVR_TIMER_OCRB }, // 9
	{ &genos::avr::timer2, AVR_TIMER_OCRA }, // 10
	{ &genos::avr::timer1, AVR_TIMER_OCRA }, // 11
	{ &genos::avr::timer1, AVR_TIMER_OCRB }, // 12
	{ &genos::avr::timer1, AVR_TIMER_OCRC }, // 13.1
	{ &genos::avr::timer0, AVR_TIMER_OCRA }  // 13.2
};
#else
#error "undefined board"
#endif

static inline
constexpr genos::avr::timer_base * arduino_pwm_timer(int num)
{
	return ARDUINO_PWM[num - 2].timer; 
}

static inline
constexpr genos::avr::timer_base * arduino_pwm_timer_configure_out(int num);

#endif