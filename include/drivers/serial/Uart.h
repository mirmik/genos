#ifndef GENOS_UART_H
#define GENOS_UART_H

#include <inttypes.h>
#include <kernel/irq.h>

#include <utilxx/setget.h>

class SerialDriver;

class UartParams {
	enum Parity {
		ParityNone,
		ParityEven,
		ParityOdd,
	};

	uint32_t baudRate;
	Parity parity;
	uint8_t stopBits;
	uint8_t dataBits;
};

class Uart {
public:
	SerialDriver* driver;

public:
	SETTER(setDriver, driver);

	using RXHandler = void(*)(void*,char);
	using TXHandler = void(*)(void*);
	using TCHandler = void(*)(void*);

	virtual int putc(char c)  = 0;
	virtual int getc()  = 0;

	int syncPutc(char c);
	int syncGetc();

	virtual int avail()  = 0;
	virtual int room()  = 0;

	virtual int enable(bool en)  = 0;
	virtual int enableRX(bool en)  = 0;
	virtual int enableTX(bool en)  = 0;

	virtual int irqEnableRX(bool en)  = 0;
	virtual int irqEnableTX(bool en)  = 0;
	virtual int irqEnableTC(bool en)  = 0;
	
	virtual int setup(UartParams *params) = 0;

	Uart() : driver(nullptr) {};
};

#endif