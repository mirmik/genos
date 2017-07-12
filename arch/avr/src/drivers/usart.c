#include <dev/usart.h>
#include <genos/panic.h>

void usart_putc(struct uart_dev* dev, char c) {
	struct usart* usart = dev->privdata;
	usart->regs->udr = c;
};

char usart_getc(struct uart_dev* dev) {
	struct usart* usart = dev->privdata;
	return usart->regs->udr;
};

uint8_t usart_txempty(struct uart_dev* dev) {
	panic("notsuported");
};
uint8_t usart_rxavail(struct uart_dev* dev) {
	panic("notsuported");
};

uint8_t usart_txint(struct uart_dev* dev, uint8_t enable) {
	panic("notsuported");
};
uint8_t usart_rxint(struct uart_dev* dev, uint8_t enable) {
	panic("notsuported");
};

void usart_setup (struct uart_dev* dev, struct uart_params params) {
	panic("notsuported");
};
struct uart_params usart_settings (struct uart_dev* dev) {
	panic("notsuported");
};

static const struct uart_ops atmega_usart_operations = {
	usart_putc, /*putc*/
	usart_getc, /*getc*/
	usart_txempty, /*txempty*/
	usart_rxavail, /*rxavail*/
	usart_txint, /*txint*/
	usart_rxint, /*rxint*/
	usart_setup, /*setup*/
	usart_settings, /*settings*/
};

const struct usart __usart0 = {USART0, (struct usart_irq_nums*)&USART0_IRQS};
const struct usart __usart1 = {USART1, (struct usart_irq_nums*)&USART1_IRQS};
const struct usart __usart2 = {USART2, (struct usart_irq_nums*)&USART2_IRQS};

const struct uart_dev usart0 = {(struct uart_params*) &uart_default_params, (struct uart_ops*) &atmega_usart_operations, (void*) &__usart0}; 
const struct uart_dev usart1 = {(struct uart_params*) &uart_default_params, (struct uart_ops*) &atmega_usart_operations, (void*) &__usart1}; 
const struct uart_dev usart2 = {(struct uart_params*) &uart_default_params, (struct uart_ops*) &atmega_usart_operations, (void*) &__usart2}; 

