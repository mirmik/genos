#ifndef GENOS_UART_H
#define GENOS_UART_H

#include <inttypes.h>

namespace Genos {
	
	namespace Uart {
		
		enum Parity {
			ParityNone,
			ParityEven,
			ParityOdd,
		};

		enum StopBits {
			StopBitsOne,
			StopBitsTwo,
		};

		enum DataBits {
			DataBitsEight,
			DataBitsSeven,
		};

	}

}

#endif