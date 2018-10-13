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

	virtual int enable(bool en) = 0;
	virtual int txirq(bool en) = 0;
	virtual int getc() = 0;
	virtual int putc(int symbol) = 0;
	virtual int hasrx() = 0;
	virtual int setup(const struct uart_params *params) = 0;
};

#endif
