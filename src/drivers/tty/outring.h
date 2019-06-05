#ifndef GENOS_OUTRING_H
#define GENOS_OUTRING_H

#include <drivers/tty/ttydrv.h>

namespace genos 
{
	class outring : public genos::ttydrv
	{
		ring_head ring;
		char * buf;

		genos::uart_device * uart;

		virtual int sendbyte(char c) override;
		virtual int begin() override;
	};
}

#endif