#ifndef GENOS_AVR_PWMSERVO_H
#define GENOS_AVR_PWMSERVO_H

#include <drivers/timer/avr_timer.h>

#include <igris/util/bug.h>

namespace genos
{
	namespace avr
	{
		struct pwmregs
		{
			genos::avr::timer_base * timer;
			uint8_t out;
		};

		class pwmservo
		{
		public:
			genos::avr::timer_base * tim;
			uint8_t compcode = -1;

		public:
			pwmservo(pwmregs pr) : tim(pr.timer), compcode(pr.out) {}

			int set_divider(int divider)
			{
				return tim->set_divider(divider);
			}

			int standart_timer_mode() 
			{
				// 20 ms period.

				dprln(tim->type());
				if (tim->type() == AVR_TIMER16) 
				{
					genos::avr::timer16 * timer16 = (genos::avr::timer16 *) tim;
					timer16->regs->icr = F_CPU / 256 / 50;
					timer16->set_mode((genos::avr::timer16::TimerMode)0b1110); 
					timer16->set_divider(256);
				} 

				else 
				{
					BUG();
				}
			}

		};
	}
}

#endif