#ifndef GXX_ARCH_HAL_GPIO_H
#define GXX_ARCH_HAL_GPIO_H

#include <hal/gpio.h>
#include <periph/regs/gpio.h>

namespace arch {
	class gpio : public hal::gpio {
		gpio_regs* regs;

	public:
		class pin : public hal::gpio::pin {
			gpio_regs* regs;
			uint8_t mask;

		public:
			pin(gpio_regs* regs, uint8_t num) : regs(regs), mask(1<<num) {}

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

			inline void mode(int32_t output) {
				if (output) regs->ddr |= mask;
				else regs->ddr &= ~mask;
			}
		};
		
		gpio(gpio_regs* regs) : regs(regs) {}

		arch::gpio::pin operator[](int num) {
			return pin(regs, num);
		} 
	};
}

#endif