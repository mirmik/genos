#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <sys/cdefs.h>
#include <defs/uartdefs.h>
#include <igris/util/retype.h>

#include <stdint.h>
#include <stdbool.h>

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1 // Можно положить следующий байт
#define UART_IRQCODE_TC 2 // Конвеер завершил свою работу.

#define UART_CTRIRQS_RXON 0
#define UART_CTRIRQS_RXOFF 1
#define UART_CTRIRQS_TXON 2
#define UART_CTRIRQS_TXOFF 3
#define UART_CTRIRQS_TCON 4
#define UART_CTRIRQS_TCOFF 5

typedef void(*uart_irq_handler_t)(void*, int);

struct uart_device
{
	const struct uart_device_operations * u_ops;

	uart_irq_handler_t  handler;
	void*               handarg;

	virtual int setup(int32_t baud, char parity, uint8_t databits, uint8_t stopbits) = 0;
	virtual int enable(int en) = 0;
	virtual int ctrirqs(uint8_t cmd) = 0;
	virtual int recvbyte() = 0;
	virtual int sendbyte(int c) = 0;
	virtual int cantx() = 0;
	virtual int hasrx() = 0;
};

#endif
