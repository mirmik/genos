#ifndef ZILLOT_STUBARCH_UART_H
#define ZILLOT_STUBARCH_UART_H

#include <zillot/common/uart.h>

namespace zillot 
{
	namespace mock 
	{
		class uart : public zillot::uart 
		{
		public:
			uart() : zillot::uart() {}
			int setup(int32_t baud, char parity, uint8_t databits, uint8_t stopbits) override { return 0; }
			int enable(int en) override { return 0; }
			int ctrirqs(uint8_t cmd) override { return 0; }
			int recvbyte() override { return 0; }
			int sendbyte(unsigned char c) override { return 0; }
			int cantx() override { return 0; }
			int hasrx() override { return 0; }
	        ~uart() override {}
		};
	}
}

#endif