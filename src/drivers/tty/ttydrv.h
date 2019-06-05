#ifndef GENOS_TTYDRV_H
#define GENOS_TTYDRV_H

namespace genos 
{
	class ldisc;

	class ttydrv 
	{
		ldisc * discipline;

		virtual int sendbyte(char c) = 0;
		virtual int begin() = 0;

		int begin(uint32_t baud, char parity, uint8_t dbits, uint8_t sbits);
		int setup_serial(uint32_t baud, char parity, uint8_t dbits, uint8_t sbits);
		int is_serial();
	};
}

#endif