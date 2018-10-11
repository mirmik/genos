#include <drivers/serial/avr_usart.h>
#include <gxx/util/member.h>
#include <hal/irqtable.h>

int avr_usart_enable(struct uart * dev, bool en) {
	struct avr_usart* usart = mcast_out(dev, struct avr_usart, u);
	usart_regs_tcirq(usart->regs, false);
	usart_regs_txirq(usart->regs, false);
	usart_regs_rxirq(usart->regs, en);
	usart_regs_enable_rx(usart->regs, en);
	usart_regs_enable_tx(usart->regs, en);
	return 0;
}

int avr_usart_txirq(struct uart * dev, bool en) {
	struct avr_usart* usart = mcast_out(dev, struct avr_usart, u);
	usart_regs_txirq(usart->regs, en);
	return 0;
}

int avr_usart_getc(struct uart * dev) {
	struct avr_usart* usart = mcast_out(dev, struct avr_usart, u);
	return usart_regs_recvbyte(usart->regs);
}

int avr_usart_putc(struct uart * dev, int symbol) {
	struct avr_usart* usart = mcast_out(dev, struct avr_usart, u);
	return usart_regs_sendbyte(usart->regs, symbol);
}

int avr_usart_hasrx(struct uart * dev) {
	struct avr_usart* usart = mcast_out(dev, struct avr_usart, u);
	return usart_regs_canrecv(usart->regs);	
}

int avr_usart_setup(struct uart * dev, const struct uart_params *params) {
	struct avr_usart* usart = mcast_out(dev, struct avr_usart, u);
	usart_regs_setup(usart->regs, params);		
	return 0;
}

static void _rx_handler(void * arg) { 
	struct avr_usart* usart = (struct avr_usart*) arg;
	if (usart->u.handler) usart->u.handler(UART_IRQCODE_RX); 
}

static void _dre_handler(void * arg) {
	struct avr_usart* usart = (struct avr_usart*) arg;
	if (usart->u.handler) usart->u.handler(UART_IRQCODE_TX); 	
}

///Прерывания опустошения конвеера. Обычно не требуется.
static void _tx_handler(void * arg) {
	struct avr_usart* usart = (struct avr_usart*) arg;
	if (usart->u.handler) usart->u.handler(UART_IRQCODE_TC); 		
}

const struct uart_operations avr_usart_operations = {
	.enable = avr_usart_enable,
	.txirq = avr_usart_txirq,
	.getc = avr_usart_getc,
	.putc = avr_usart_putc,
	.hasrx = avr_usart_hasrx,
	.setup = avr_usart_setup
};

int avr_usart_init(struct avr_usart * dev, struct usart_regs * regs, int base_irq) {
	dev->regs = regs;
	dev->base_irq = base_irq;
	dev->u.u_op = &avr_usart_operations;
	
	irq_set_handler(base_irq    , _rx_handler, dev);
	irq_set_handler(base_irq + 2, _dre_handler, dev);
	irq_set_handler(base_irq + 1, _tx_handler, dev);

	return 0;
}