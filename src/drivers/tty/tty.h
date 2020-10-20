#ifndef GENOS_TTY_H
#define GENOS_TTY_H

#include <genos/resource.h>
#include <drivers/serial/uart.h>

class tty : public genos::node
{
	drivers::uart * uart;
	drivers::tty_disc * disc;

public:
	tty(drivers::uart * uart, drivers::tty_disc * disc) :
		tty(uart, disc)
	{}


	int release() override
	{
		return ENOTSUP;
	}

	int open(openres * ores) override
	{
		return ENOTSUP;
	}


	int write(const void* data, size_t size, int flags) override
	{
		return disc->write(data, size, flags);
	}

	int read(void* data, size_t size, int flags) override
	{
		return disc->read(data, size, flags);
	}

	int room() override
	{
		return disc->room();
	}

	int avail() override
	{
		return disc->avail();
	}


	void rx_interrupt_handle()
	{
		disc->newchar_handle(uart->recvbyte());
	}

	void tx_interrupt_handle(char c)
	{
		disc->tx_request_handle(c);
	}
};

#endif