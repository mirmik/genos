#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <sys/cdefs.h>
#include <drivers/serial/settings.h>
#include <stdbool.h>

struct uart;

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1 // Можно положить следующий байт
#define UART_IRQCODE_TC 2 // Конвеер завершил свою работу.

///uart bottom-half driver
struct uart {
	void(*handler)(void*, int);
	void * handarg;

	virtual int enable(bool en=true) = 0;
	virtual int txirq(bool en) = 0;
	virtual int recvbyte() = 0;
	virtual int sendbyte(int symbol) = 0;
	virtual int cantx() = 0;
	virtual int hasrx() = 0;
	//virtual int setup(const struct uart_params *params) = 0;
	virtual int setup(int32_t baud, enum uart_parity_e parity, uint8_t databits, uint8_t stopbits) = 0;

	int begin(int32_t baud, enum uart_parity_e parity=UART_PARITY_NONE, uint8_t databits=8, uint8_t stopbits=1) 
	{
		int ret = setup(baud, parity, databits, stopbits);
		txirq(false);
		enable(true);
		return ret;
	}
};

#endif
