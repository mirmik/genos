#include <periph/dev/usart.h>
#include <util/bits.h>

int usart_avail(struct usart_device* usart) {
	return bits_mask(usart->regs->ucsr_a, 1 << 6);	
}

int usart_ready(struct usart_device* usart) {
	return bits_mask(usart->regs->ucsr_a, 1 << 5);	
}

int usart_putc(struct uart_driver* uart, char c) {
	struct usart_device* usart = uart->priv;

	if (ring_is_empty(&usart->txring) && usart_ready(usart)) {
		usart->regs->udr = c;
		return 1;
	}

	if (ring_room_size(&usart->txring) == 0) panic("NOROOM");

	ring_putc(&usart->txring, usart->txbuffer, c);
	return 1;
}

int usart_getc(struct uart_driver* uart) {
	struct usart_device* usart = uart->priv;
}

int usart_init(struct uart_driver* uart) {
	struct usart_device* usart = uart->priv;

	bits_lvl(usart->regs->ucsr_b, 1 << 7, 1); //rx interrupt enable
	bits_lvl(usart->regs->ucsr_b, 1 << 6, 1); //tx interrupt enable
	bits_lvl(usart->regs->ucsr_b, 1 << 4, 1); //rx enable
	bits_lvl(usart->regs->ucsr_b, 1 << 3, 1); //tx enable
}

void usart_rx_handler(struct usart_device* dev) {
	panic("RX");
}

void usart_tx_handler(struct usart_device* dev) {
	if (!ring_is_empty(&dev->txring)) dev->regs->udr = ring_getc(&dev->txring, dev->txbuffer);
}

void usart_udre_handler(struct usart_device* dev) {

}

struct uart_ops usart_operations = {
	usart_putc, //int (*putc)(char c);
	usart_getc, //int (*getc)();
	usart_init,//int (*init)();
};

void usart_driver_register(struct uart_driver* uart, struct usart_device* dev) {
	struct usart_device* usart = uart->priv;

	uart -> priv = dev;
	uart -> ops  = &usart_operations;

	ring_init(&usart->txring, USART_TXBUFSIZE);
	setIRQHandler(dev->irqs.rx, (IRQHandler) usart_rx_handler, dev);
	setIRQHandler(dev->irqs.tx, (IRQHandler) usart_tx_handler, dev);
	setIRQHandler(dev->irqs.udre, (IRQHandler) usart_udre_handler, dev);
}
