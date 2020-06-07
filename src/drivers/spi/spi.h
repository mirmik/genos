#ifndef GENOS_DRIVERS_SPI_H
#define GENOS_DRIVERS_SPI_H

#include <sys/cdefs.h>
#include <sched/flags.h>

#include <igris/datastruct/dlist.h>
#include <igris/util/bug.h>

#include <drivers/gpio/pin.h>

struct spi_operations;

enum spi_mode_e {
	SPI_MODE_0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

namespace genos
{
	class spi
	{
    	uint8_t locked;
    	dlist_head waits;

    public:
    	virtual int begin() = 0;

		virtual int enable 			(bool en = true) = 0;
//		virtual int select   		(void *slct, int en); // device must select himself
		virtual int exbyte 			(int byte) = 0;

		// Произвести обмен по шине spi.
		// При этом, 
		// если txbuf==NULL, вместо данных спамится байт dummy
		// если rxbuf==NULL, процедура чтения опускается
		virtual int exchange 		(const void *txbuf, void *rxbuf, int len, char dummy = '\0') = 0;
		
		virtual int setfrequency	(uint32_t freq) 
		{
			BUG();
		}

		void lock() 
		{ 
			// waiting TODO
			locked = 1;  
		}
		
		void unlock() 
		{ 
			// waiting TODO
			locked = 0;  
		}

		//virtual void init(const char * name);
	};

	class spi_selectable_device 
	{
		genos::spi * driver;
		gpio_pin cs;

	public:
		spi_selectable_device(genos::spi * driver, gpio_pin slct) 
			: driver(driver), cs(slct) 
		{
			init_select_pin();
		}

		void init_select_pin() 
		{
			cs.mode(GPIO_MODE_INPUT | GPIO_MODE_IN_PULL_UP);
			cs.set(1);
		}

		void lock_bus();
		void unlock_bus();

		int select(bool en = true) { cs.set(!en); return 0; }
		int deselect() { return select(false); }
	};
}

#endif