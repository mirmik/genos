#include <drivers/serial/avr_usart.h>
#include <gxx/util/member.h>
#include <hal/irqtable.h>

int avr_usart::enable(bool en) {
	usart_regs_enable_rx(regs, en);
	usart_regs_enable_tx(regs, en);
	usart_regs_tcirq(regs, false);
	usart_regs_txirq(regs, false);
	usart_regs_rxirq(regs, en);
	return 0;
}

int avr_usart::txirq(bool en) {
	usart_regs_txirq(regs, en);
	return 0;
}

int avr_usart::getc() {
	return usart_regs_recvbyte(regs);
}

int avr_usart::putc(int symbol) {
	return usart_regs_sendbyte(regs, symbol);
}

int avr_usart::hasrx() {
	return usart_regs_canrecv(regs);	
}

int avr_usart::setup(const struct uart_params *params) {
	usart_regs_setup(regs, params);		
	return 0;
}


static void _rx_handler(void * arg) { 
	struct avr_usart* usart = (struct avr_usart*) arg;
	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_RX); 
}

static void _dre_handler(void * arg) {
	struct avr_usart* usart = (struct avr_usart*) arg;
	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_TX); 	
}

///Прерывания опустошения конвеера. Обычно не требуется.
static void _tx_handler(void * arg) {
	struct avr_usart* usart = (struct avr_usart*) arg;
	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_TC); 		
}

int avr_usart::init(struct usart_regs * _regs, int _base_irq) {
	regs = _regs;
	base_irq = _base_irq;
	
	irq_set_handler(base_irq    , _rx_handler, (void*)this);
	irq_set_handler(base_irq + 1, _dre_handler, (void*)this);
	irq_set_handler(base_irq + 2, _tx_handler, (void*)this);

	return 0;
}