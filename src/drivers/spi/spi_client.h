#ifndef GENOS_DRIVERS_SPI_CLIENT_H
#define GENOS_DRIVERS_SPI_CLIENT_H

namespace genos 
{
	class spi_client : public genos::device
	{
		genos::spi_device * driver;
		gpio_pin cs;

	public:
		device * parent_device() override { return driver; }

		spi_client(genos::spi * driver, gpio_pin slct)
			: driver(driver), cs(slct)
		{
			if (cs.gpio != nullptr)
				init_select_pin();
		}

		void init_select_pin()
		{
			cs.mode(GPIO_MODE_INPUT | GPIO_MODE_IN_PULL_UP);
			cs.set(1);
		}

		void lock_bus() { driver->lock_bus(); }
		void unlock_bus() { driver->unlock_bus(); }

		int select(bool en = true) { cs.set(!en); return 0; }
		int deselect() { return select(false); }
	};
}

#endif