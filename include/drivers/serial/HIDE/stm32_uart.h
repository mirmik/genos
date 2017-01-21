#ifndef STM32_UART_SERIAL_DEVICE
#define STM32_UART_SERIAL_DEVICE

#include <compiler.h>

#include <periph/regs/usart.h>

#include <drivers/serial/uart_device.h>
#include <communication/serial/serial_driver.h>

struct stm32_uart {
	/*parrent*/
	struct uart parent; 
	
	/*data*/
	struct usart_regs* regs;

	/*irqs numbers*/
	uint8_t irqno;
};

__BEGIN_DECLS

extern struct uart_ops stm32_uart_ops;
int stm32_uart_init(struct stm32_uart* su, struct usart_regs* regs, uint8_t irqno);
int stm32_uart_irqs_setup(struct stm32_uart* su);

//void stm32_uart_irq_rx_handler(void* uart);
//void stm32_uart_irq_tx_handler(void* uart);
//void stm32_uart_irq_tx_complete_handler(void* uart);

__END_DECLS

#define STM32_UART_DEF(name,_regs)  \
struct stm32_uart name = {			\
	.parent = {						\
		.uops = &stm32_uart_ops,	\
	},								\
	.regs = _regs,					\
};

#endif