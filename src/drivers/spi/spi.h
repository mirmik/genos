/*#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <igris/compiler.h>
#include <igris/util/bug.h>
#include <igris/sync/semaphore.h>

#include <drivers/gpio/pin.h>
#include <drivers/device.h>

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
	class spi_driver : public genos::device
	{
		struct semaphore __lock_bus = SEMAPHORE_INIT(__lock_bus, 1);

	public:
		device * parent_device() override { return nullptr; }

		virtual int begin() = 0;

		virtual uint8_t recvbyte() = 0;
		virtual void sendbyte(uint8_t b) = 0;

//		virtual int enable (bool en = true) = 0;
//		virtual int select   		(void *slct, int en); // device must select himself
//		virtual int exbyte (int byte) = 0;

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

		void lock_bus() { semaphore_down(&__lock_bus); }
		void unlock_bus() { semaphore_up(&__lock_bus); }
	};

	
	class spi_automate
	{

	};
}

#endif*/