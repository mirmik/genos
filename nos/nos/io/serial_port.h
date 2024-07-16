#ifndef NOS_IO_SERIAL_PORT_H
#define NOS_IO_SERIAL_PORT_H

/**
	@file
*/

#include <nos/io/file.h>
#include <unistd.h>

#define UART_PARITY_NONE 'n'
#define UART_PARITY_EVEN 'e'
#define UART_PARITY_ODD  'o'

#define UART_STOPBITS_ONE 1
#define UART_STOPBITS_TWO 2

namespace nos
{
	class serial_port : public nos::file
	{
		unsigned int baud=9600;
		char parity='n';
		uint8_t bytesize=0;
		uint8_t stopbits=0;

	public:
		serial_port() {}
		serial_port(int fd)
			: nos::file(fd)
		{}

		serial_port(const char * path,
		            unsigned int baud = 9600,
		            char parity = UART_PARITY_NONE,
		            uint8_t bytesize = 8,
		            uint8_t stopbits = UART_STOPBITS_ONE)
			: baud(baud),
			  parity(parity),
			  bytesize(bytesize),
			  stopbits(stopbits)
		{
			open(path,
			     baud,
			     parity,
			     bytesize,
			     stopbits);
		}

		int open(const char * path,
		         unsigned int baud = 9600,
		         char parity = UART_PARITY_NONE,
		         uint8_t bytesize = 8,
		         uint8_t stopbits = UART_STOPBITS_ONE);

		int setup(unsigned int baud = 9600,
		         char parity = UART_PARITY_NONE,
		         uint8_t bytesize = 8,
		         uint8_t stopbits = UART_STOPBITS_ONE);

		void reconfigurePort();
	};
}

#endif
