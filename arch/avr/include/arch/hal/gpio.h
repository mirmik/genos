#ifndef GXX_ARCH_HAL_GPIO_H
#define GXX_ARCH_HAL_GPIO_H

#include <periph/regs/gpio.h>

namespace hal {
	class gpio {
		gpio_regs* regs;

	public:
		class pin {
			gpio_regs* regs;
			uint8_t mask;

		public:
			pin(gpio_regs* regs, uint8_t mask) : regs(regs), mask(mask) {}

			inline void set() {
				regs->port |= mask;
			}

			inline void clr() {
				regs->port &= ~mask;
			}

			inline void lvl(bool en) {
				if (en) regs->port |= mask;
				else regs->port &= ~mask;
			}

			inline void tgl() {
				regs->pin = mask;
			}

			inline void mode(bool output) {
				if (output) regs->ddr |= mask;
				else regs->ddr &= ~mask;
			}
		};
		
		gpio(gpio_regs* regs) : regs(regs) {}

		pin operator[](int i) {
			return pin(regs, 1 << i);
		} 

		constexpr static uint8_t input = 0;
		constexpr static uint8_t output = 1;
	};
}

#endif