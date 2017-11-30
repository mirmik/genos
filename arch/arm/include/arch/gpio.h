#ifndef GXX_ARM_ARCH_HAL_GPIO_H
#define GXX_ARM_ARCH_HAL_GPIO_H

#include <hal/gpio.h>
#include <periph/regs/port.h>

namespace arch {
	class gpio : public hal::gpio {
		port_group_regs* regs;

	public:
		class pin : public hal::gpio::pin {
			port_group_regs* regs;
			uint32_t mask;

		public:
			pin(port_group_regs* regs, uint8_t num) : regs(regs), mask(1<<num) {}

			inline void set() {
				regs->OUTSET |= mask;
			}

			inline void clr() {
				regs->OUTCLR |= mask;
			}

			inline void lvl(bool en) {
				if (en) set();
				else clr();
			}

			inline void tgl() {
				regs->OUTTGL = mask;
			}

			inline void mode(int32_t output) {
				if (output) regs->DIR |= mask;
				else regs->DIR &= ~mask;
			}
		};
		
		gpio(port_group_regs* regs) : regs(regs) {}

		arch::gpio::pin operator[](int num) {
			return pin(regs, num);
		} 
	};
}

#endif