#ifndef GENOS_DRIVERS_CDEV_TTY_H
#define GENOS_DRIVERS_CDEV_TTY_H

#include <genos/cdev.h>

#include <drivers/tty/ttydrv.h>
#include <drivers/tty/ldisc.h>

namespace genos 
{
	class tty : public char_device
	{
	public:
		uart_device * driver;
		genos::ldisc * ldisc;

		void set_driver(uart_device * u) { driver = u; } 
		void set_ldisc(genos::ldisc * l) { ldisc = l; }

		int write(const char* data, size_t size) override
		{
			return ldisc->write(data, size);
		}

		int read(char* data, size_t size) 
		{
			return ldisc->read(data, size);
		}
	};
}

#endif