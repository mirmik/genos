#include <drivers/serial/avr_usart.h>
//#include </util/member.h>

#include <hal/irqtable.h>
#include <hal/board.h>

#include <util/delay.h>

int avr_usart::enable(bool en) {
	usart_regs_enable_rx(regs, en);
	usart_regs_enable_tx(regs, en);
	usart_regs_tcirq(regs, false);
	usart_regs_txirq(regs, false);
	usart_regs_rxirq(regs, en);

	cpu_delay(10000);

	return 0;
}

int avr_usart::txirq(bool en) {
	usart_regs_txirq(regs, en);
	return 0;
}

int avr_usart::recvbyte() {
	return usart_regs_recvbyte(regs);
}

int avr_usart::sendbyte(int symbol) {
	return usart_regs_sendbyte(regs, symbol);
}

int avr_usart::cantx() {
	return usart_regs_cansend(regs);	
}

int avr_usart::hasrx() {
	return usart_regs_canrecv(regs);	
}

int avr_usart::setup(int32_t baud, enum uart_parity_e parity, uint8_t databits, uint8_t stopbits) {
	usart_regs_setup(regs, baud, parity, databits, stopbits);		
	irqinit();
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

int avr_usart::irqinit() {
	irq_set_handler(base_irq    , _rx_handler, (void*)this);
	irq_set_handler(base_irq + 1, _dre_handler, (void*)this);
	irq_set_handler(base_irq + 2, _tx_handler, (void*)this);

	return 0;
}