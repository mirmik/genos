#ifndef GENOS_DRIVERS_STM32_USART_H
#define GENOS_DRIVERS_STM32_USART_H

#include <drivers/serial/uart.h>
#include <periph/irqdefs.h>
#include <asm/usart.h>
#include <asm/nvic.h>

#include <hal/irqtable.h>

__BEGIN_DECLS

int stm32_usart_device_enable(struct uart_device * u, int en);
int stm32_usart_device_ctrirqs(struct uart_device * u, uint8_t cmd);
int stm32_usart_device_recvbyte(struct uart_device * u);
int stm32_usart_device_sendbyte(struct uart_device * u, int b);
int stm32_usart_device_cantx(struct uart_device * u);
int stm32_usart_device_hasrx(struct uart_device * u);
int stm32_usart_device_setup(struct uart_device * u,
                             int32_t baud, char parity,
                             uint8_t databits, uint8_t stopbits);

__END_DECLS

extern const struct uart_device_operations stm32_usart_device_operations;

struct stm32_usart_device
{
	struct uart_device dev;
	
	struct usart_regs * regs;
	uint8_t irqno;
};

#define STM32_USART_DEVICE_DECLARE(name, regs, irqno)	\
struct stm32_usart_device name = {{&stm32_usart_device_operations}, regs, irqno}

#endif