#ifndef GENOS_GPROTOCOL_H
#define GENOS_GPROTOCOL_H

#include <genos/sigslot/delegate.h>

#define STRTBYTE 0x7E
#define ESCBYTE 0x7D
#define BYTECODE_7E 0x5E
#define BYTECODE_7D 0x5D

class UartRingSender {
	uart_t* uart;
	void send(char c);
};

class StuffDriver {
	UartRingSender
};

#endif