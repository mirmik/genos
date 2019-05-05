#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

__BEGIN_DECLS

int avr_usart_device_enable(struct uart_device * u, int en);
int avr_usart_device_ctrirqs(struct uart_device * u, uint8_t cmd);
int avr_usart_device_recvbyte(struct uart_device * u);
int avr_usart_device_sendbyte(struct uart_device * u, int b);
int avr_usart_device_cantx(struct uart_device * u);
int avr_usart_device_hasrx(struct uart_device * u);
int avr_usart_device_setup(struct uart_device * u,
                             int32_t baud, char parity,
                             uint8_t databits, uint8_t stopbits);

__END_DECLS

extern const struct uart_device_operations avr_usart_device_operations;

struct avr_usart_device
{
	struct uart_device dev;
	
	struct usart_regs * regs;
	uint8_t base_irq;
};

#define AVR_USART_DEVICE_DECLARE(name, regs, irqno)	\
struct avr_usart_device name = {{&avr_usart_device_operations}, regs, irqno}

#endif