#ifndef ARDUINO_PWM_H
#define ARDUINO_PWM_H

#include <drivers/timer/avr_timer.h>
#include <drivers/pwmservo/avr_pwmservo.h>

#ifdef BOARD_ARDUINO_MEGA
constexpr const struct genos::avr::pwmregs ARDUINO_PWM[] = 
{
	{ &genos::avr::timer3, 'b' }, // 2
	{ &genos::avr::timer3, 'c' }, // 3
	{ &genos::avr::timer0, 'b' }, // 4
	{ &genos::avr::timer3, 'a' }, // 5
	{ &genos::avr::timer4, 'a' }, // 6
	{ &genos::avr::timer4, 'b' }, // 7
	{ &genos::avr::timer4, 'c' }, // 8
	{ &genos::avr::timer2, 'b' }, // 9
	{ &genos::avr::timer2, 'a' }, // 10
	{ &genos::avr::timer1, 'a' }, // 11
	{ &genos::avr::timer1, 'b' }, // 12
	{ &genos::avr::timer1, 'c' }, // 13.1
	{ &genos::avr::timer0, 'a' }  // 13.2
};
#endif

static inline
constexpr genos::avr::pwmregs arduino_pwm_timer(int num)
{
#ifdef BOARD_ARDUINO_MEGA
	return ARDUINO_PWM[num - 2];
#elif BOARD_ARDUINO_UNO
	switch(num) 
	{
		case 3:  return { &genos::avr::timer2, 'b' };
		case 5:  return { &genos::avr::timer0, 'b' };
		case 6:  return { &genos::avr::timer0, 'a' };
		case 9:  return { &genos::avr::timer1, 'a' };
		case 10: return { &genos::avr::timer1, 'b' };
		case 11: return { &genos::avr::timer2, 'a' };
	}
#endif	 
	return {nullptr, 0};
}

#ifdef BOARD_ARDUINO_MEGA
#define GET_ARDUINO_PWM(n) (ARDUINO_PWM[(n) - 2])
#endif	 

static inline
constexpr genos::avr::timer_base * arduino_pwm_timer_configure_out(int num);

#endif