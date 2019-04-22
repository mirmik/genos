#ifndef GENOS_DRIVERS_XPIN_H
#define GENOS_DRIVERS_XPIN_H

#include <drivers/gpio/gpio.h>

namespace genos
{
	namespace drivers
	{
		class xpin
		{
			gpio_t * gpio;
			gpio_mask_t mask;

		public:
			xpin(gpio_t * gpio, gpio_mask_t no) : gpio(gpio), mask(1 << no) {};

			int settings(uint32_t mode)
			{
				return gpio_settings(gpio, mask, mode);
			}

			void write(uint8_t lvl)
			{
				gpio_write(gpio, mask, lvl);
			}

			void toggle()
			{
				gpio_toggle(gpio, mask);
			}

			uint8_t read()
			{
				return !!gpio_read(gpio, mask);
			}
		};
	}
}

#endif