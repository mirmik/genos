#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <sys/cdefs.h>
#include <sched/flags.h>

struct spi_operations;

enum spi_mode_e {
	SPI_MODE_0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

namespace genos 
{
	struct spi_driver
	{
   		//const struct spi_operations * spi_op;
    	uint8_t locked;

		virtual int enable 			(bool en = true);
		virtual int select   		(void *slct, int en);
		virtual int exbyte 			(int byte);
		virtual int exchange 		(const void *txbuf, void *rxbuf, int len);
		virtual int setfrequency	(uint32_t freq);

		void lock() { locked = 1; }
		void unlock() { locked = 0; }

		//virtual void init(const char * name);
	};
}

#endif