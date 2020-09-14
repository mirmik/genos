#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <sys/cdefs.h>
#include <sched/flags.h>

#include <igris/datastruct/dlist.h>
#include <igris/util/bug.h>

#include <drivers/gpio/pin.h>
#include <drivers/bus.h>

struct spi_operations;

enum spi_mode_e
{
	SPI_MODE_0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

namespace genos
{
	class spi_operations 
	{
		// Произвести обмен по шине spi.
		// При этом,
		// если txbuf==NULL, вместо данных спамится байт dummy
		// если rxbuf==NULL, процедура чтения опускается
		virtual int exchange (
		    const void *txbuf,
		    void *rxbuf,
		    int len) = 0;
	}

	class spi_device : public genos::device, public spi_operations
	{
		struct semaphore __lock_bus = SEMAPHORE_INIT(__lock_bus);

	public:
		device * parent_device() override { return nullptr; }

		virtual int begin() = 0;

		virtual int enable (bool en = true) = 0;
//		virtual int select   		(void *slct, int en); // device must select himself
		virtual int exbyte (int byte) = 0;

		// Произвести обмен по шине spi.
		// При этом,
		// если txbuf==NULL, вместо данных спамится байт dummy
		// если rxbuf==NULL, процедура чтения опускается
		virtual int exchange (
		    const void *txbuf,
		    void *rxbuf,
		    int len) = 0;

		virtual int setfrequency (uint32_t freq)
		{
			BUG();
		}

		void lock_bus();
		void unlock_bus();
	};

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

	class spi_automate
	{

	};
}

#endif