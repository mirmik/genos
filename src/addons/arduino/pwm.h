#ifndef ARDUINO_PWM_H
#define ARDUINO_PWM_H

#include <periph/regs/timer.h>

struct pwmregs 
{
	void * timer;
	volatile void * reg;
	uint8_t type;
};

#ifdef BOARD_ARDUINO_MEGA
constexpr const struct pwmregs ARDUINO_PWMS[] = 
{
	{ TIMER3, &TIMER3->ocr_b, 0 }, // 2
	{ TIMER3, &TIMER3->ocr_c, 0 }, // 3
	{ TIMER0, &TIMER0->ocr_b, 0 }, // 4
	{ TIMER3, &TIMER3->ocr_a, 0 }, // 5
	{ TIMER4, &TIMER4->ocr_a, 0 }, // 6
	{ TIMER4, &TIMER4->ocr_b, 0 }, // 7
	{ TIMER4, &TIMER4->ocr_c, 0 }, // 8
	{ TIMER2, &TIMER2->ocr_b, 0 }, // 9
	{ TIMER2, &TIMER2->ocr_a, 0 }, // 10
	{ TIMER1, &TIMER1->ocr_a, 0 }, // 11
	{ TIMER1, &TIMER1->ocr_b, 0 }, // 12
	{ TIMER1, &TIMER1->ocr_c, 0 }, // 13.1
	{ TIMER0, &TIMER0->ocr_a, 0 }  // 13.2
};
#else
#error "undefined board"
#endif

static inline
constexpr pwmregs arduino_pwm_regs(int num) 
{
	return ARDUINO_PWMS[num - 2]; 
}

#endif