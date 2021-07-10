#ifndef DRIVERS_TTY_DISC_H
#define DRIVERS_TTY_DISC_H

namespace drivers 
{
	class tty_disc 
	{
		virtual ssize_t write(const void* data, size_t size, int flags) = 0;
		virtual ssize_t read(const void* data, size_t size, int flags) = 0;

		virtual int tx_request_handle() = 0;
		virtual void newchar_handle(int c) = 0;
	};
}

#endif