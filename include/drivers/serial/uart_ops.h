#ifndef GENOS_DRIVERS_UART_OPS
#define GENOS_DRIVERS_UART_OPS

struct uart_params;

typedef int(*uart_putc_t)(struct chardev_s*, char);
typedef int(*uart_getc_t)(struct chardev_s*);
typedef int(*uart_avail_t)(struct chardev_s*);
typedef int(*uart_room_t)(struct chardev_s*);

typedef int(*uart_setup_t)(struct chardev_s*, struct uart_params* params);

enum uart_parity {
	UartParityNone,
	UartParityEven,
	UartParityOdd,
};	

struct uart_params {
	uint32_t baudRate;
	uart_parity parity;
	uint8_t stopBits;
	uint8_t dataBits;
};

struct uart_ops {
	uart_putc_t putc;
	uart_getc_t getc;
	uart_avail_t avail;
	uart_room_t room;

	uart_setup_t setup;
};

#endif