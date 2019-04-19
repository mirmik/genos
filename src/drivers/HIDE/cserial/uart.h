#ifndef DRIVERS_SERIAL_UART_H
#define DRIVERS_SERIAL_UART_H

#include <sys/cdefs.h>

//typedef int(*)(bool) uart_enable_t;

typedef int(*)(int32_t baud, enum uart_parity_e parity,
               uint8_t databits, uint8_t stopbits) uart_setup_t;
typedef int(*)(bool) uart_txirq_t;
typedef int(*)(int) uart_send_t;
typedef int(*)() uart_recv_t;
typedef int(*)() uart_cantx_t;
typedef int(*)() uart_hasrx_t;

struct uart_device_operations
{
	uart_send_t send;
	uart_recv_t recv;

	uart_hasrx_t hasrx;
	uart_cantx_t cantx;

	uart_txirq_t txirq;
	uart_setup_t setup;
};

struct uart_device
{
	void(*handler)(void*, int);
	void * handarg;

	const uart_device_operations * ops;
};

static inline void uart_device_init(struct uart_device* dev,)

#endif