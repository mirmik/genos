#ifndef GENOS_AVR_USART_DEV_H
#define GENOS_AVR_USART_DEV_H

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

/*void usart_putc(struct uart_dev* dev, char c);
char usart_getc(struct uart_dev* dev);

uint8_t usart_txempty(struct uart_dev* dev);
uint8_t usart_rxavail(struct uart_dev* dev);

uint8_t usart_txint(struct uart_dev* dev, uint8_t enable);
uint8_t usart_rxint(struct uart_dev* dev, uint8_t enable);

void usart_setup (struct uart_dev*, struct uart_params params);
struct uart_params usart_settings (struct uart_dev*);
*/
struct usart {
	volatile struct usart_regs* regs;
	struct usart_irq_nums* 		irqs;
};

__BEGIN_DECLS

extern const struct usart __usart0;
extern const struct usart __usart1;
extern const struct usart __usart2;

extern const struct uart_dev usart0;
extern const struct uart_dev usart1;
extern const struct uart_dev usart2;

__END_DECLS

#endif