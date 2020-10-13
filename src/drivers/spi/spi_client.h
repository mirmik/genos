#ifndef GENOS_DRIVERS_SPI_CLIENT_H
#define GENOS_DRIVERS_SPI_CLIENT_H

#include <drivers/spi/spi_device.h>
#include <drivers/gpio/pin.h>

namespace genos 
{
	class spi_client : public genos::device
	{
		genos::spi_device * parent;
		gpio_pin * cs;

	public:
		device * parent_device() override { return parent; }

		spi_client(genos::spi_device * dev, gpio_pin * slct)
			: parent(dev), cs(slct)
		{
			if (cs != nullptr)
				init_select_pin();
		}

		void init_select_pin()
		{
			cs->mode(GPIO_MODE_INPUT | GPIO_MODE_IN_PULL_UP);
			cs->set(1);
		}

		int exchange(void * tx, void * rx, int len) 
		{
			//dprln("exchange");
			return parent->exchange(tx, rx, len);
			//dprln("exchange ... ok");
		}

		void lock_bus() { parent->lock_bus(); }
		void unlock_bus() { parent->unlock_bus(); }

		int select(bool en = true) { cs->pull_up(!en); return 0; }
		int deselect() { return select(false); }
	};
}

#endif