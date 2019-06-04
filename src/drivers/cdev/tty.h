#ifndef GENOS_DRIVERS_CDEV_TTY_H
#define GENOS_DRIVERS_CDEV_TTY_H

#include <genos/resource.h>

#include <drivers/serial/uart.h>
#include <drivers/cdev/ldisc.h>

namespace genos 
{
	class tty : public node
	{
	public:
		uart_device * driver;
		genos::ldisc * ldisc;

		void set_driver(uart_device * u) { driver = u; } 
		void set_ldisc(genos::ldisc * l) { ldisc = l; }
	};
}

#endif