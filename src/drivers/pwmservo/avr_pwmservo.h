#ifndef GENOS_AVR_PWMSERVO_H
#define GENOS_AVR_PWMSERVO_H

#include <drivers/timer/avr_timer.h>

#include <igris/util/bug.h>

namespace genos
{
	namespace avr
	{
		template<class T>
		class pwmservo_writer
		{
			volatile T* reg;

			T regmin;
			T regmax;

		public:
			pwmservo_writer(){}

			pwmservo_writer(const pwmservo_writer& oth) : 
				reg(oth.reg), regmin(oth.regmin), regmax(oth.regmax)
			{}

			pwmservo_writer(volatile T* reg, T regmax, T regmin) :
				reg(reg), regmin(regmin), regmax(regmax) {}

			void set(float p)
			{
				assert(p >= 0 && p <= 1);
				*reg = (float)regmin * (1 - p) + (float)regmax * p;
			}
		};

		struct pwmregs
		{
			genos::avr::timer_base * timer;
			char out;
		};

		class pwmservo
		{
		public:
			genos::avr::timer16 * tim;
			char compcode = -1;

		public:

			pwmservo(){}

			pwmservo(pwmregs pr) : tim((genos::avr::timer16 *)pr.timer), compcode(pr.out)
			{
				assert(pr.timer->type() == AVR_TIMER16);
			}

			pwmservo(const pwmservo& oth) : tim(oth.tim), compcode(oth.compcode)
			{}

			int set_divider(int divider)
			{
				return tim->set_divider(divider);
			}

			int standart_timer_mode()
			{
				// 20 ms period.
				genos::avr::timer16 * timer16 = (genos::avr::timer16 *) tim;
				timer16->regs->icr = F_CPU / 256 / 50;
				timer16->set_mode((genos::avr::timer16::TimerMode)0b1110);
				timer16->set_divider(256);
				return 0;
			}

			volatile uint16_t * pwm_mode_start()
			{
				volatile uint16_t * ocr = tim->get_ocr_register(compcode);
				tim->enable_ocr_out(compcode);
				*ocr = 0xFFFF;
				return ocr;
			}

			pwmservo_writer<uint16_t> get_writer(uint16_t usmin, uint16_t usmax) 
			{
				volatile uint16_t * ocr = tim->get_ocr_register(compcode);
				uint16_t ocrmax = (float)tim->regs->icr * usmax / 20000;
				uint16_t ocrmin = (float)tim->regs->icr * usmin / 20000;
				return { ocr, ocrmin, ocrmax };
			}
		};
	}
}

#endif