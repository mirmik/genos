#ifndef GENOS_DRIVERS_AVR_TIMER_H
#define GENOS_DRIVERS_AVR_TIMER_H

#include <periph/regs/timer.h>

namespace genos
{
	namespace avr
	{
		struct timer_irqs 
		{
			uint8_t ovf;
		};

		static uint8_t tc_divider_code(uint16_t divider)
		{
			switch (divider)
			{
				case 0 : return 0;
				case 1 : return 1;
				case 8 : return 2;
				case 64 : return 3;
				case 256 : return 4;
				//default: BUG("wrong_divider");
			}
			return -1;
		}

		template<class T, class R>
		struct timer 
		{
			R  							regs;
			struct timer_irqs	 		irqs;
			volatile uint8_t* 			timsk;

			T value() 
			{ 
				return regs->tcnt; 
			}
			
			void set_value(T val) 
			{ 
				regs->tcnt = val; 
			}

			int enable(uint8_t div) 
			{
				int divcode = tc_divider_code(div);
				if (divcode < 0) 
					return -1;
				
				bits_mask_assign(regs->tccr_b, 7, divcode); 
				return 0;
			}

			int irq_ovf_enable(bool en) 
			{
				bits_mask_assign(*timsk, 0x1, en);	
			}
		};

		struct timer8 : public timer<uint8_t, timer_8bit_regs*>
		{};

		struct timer16 : public timer<uint16_t, timer_16bit_regs*>
		{};
	}
}

#endif