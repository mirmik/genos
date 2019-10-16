#ifndef MCP23S08_DEVICE_DRIVER_H
#define MCP23S08_DEVICE_DRIVER_H

#include <drivers/spi/spi.h>

namespace drivers
{
	class mcp23s08_device : public drivers::spi_selectable_device
	{
	public:
		mcp23s08_device(drivers::spi_driver * spi, gpio_pin slct) :
			drivers::spi_selectable_device(spi, slct)
		{}

		void init()
		{
			uint8_t A0 = 0;
			uint8_t A1 = 0;

			const char buf[] = { 0, 1, 0, 0, 0, A0, A1 };
			//spi->exchange(buf, nullptr, 7);
		}

		int test() 
		{
			int ret = -1;
			select();

			ret = 0;

			deselect();
			return ret;
		}
	};
}

#endif