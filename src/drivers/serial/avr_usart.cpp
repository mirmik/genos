#include <drivers/serial/avr_usart.h>
//#include </util/member.h>

#include <hal/irqtable.h>
#include <hal/board.h>

#include <util/delay.h>

int genos::drivers::avr_usart::enable(bool en)
{
	usart_regs_enable_rx(regs, en);
	usart_regs_enable_tx(regs, en);
	usart_regs_tcirq(regs, false);
	usart_regs_txirq(regs, false);
	usart_regs_rxirq(regs, en);

	cpu_delay(10000);

	return 0;
}

int genos::drivers::avr_usart::ctrirqs(uint8_t cmd)
{
	switch (cmd)
	{
		case UART_CTRIRQS_TXON: usart_regs_txirq(regs, true); return 0;

		case UART_CTRIRQS_TXOFF: usart_regs_txirq(regs, false); return 0;

		case UART_CTRIRQS_RXON: usart_regs_rxirq(regs, true); return 0;

		case UART_CTRIRQS_RXOFF: usart_regs_rxirq(regs, false); return 0;

		case UART_CTRIRQS_TCON: usart_regs_tcirq(regs, true); return 0;

		case UART_CTRIRQS_TCOFF: usart_regs_tcirq(regs, false); return 0;
	}

	return -1;
}

int genos::drivers::avr_usart::recvbyte()
{
	return usart_regs_recvbyte(regs);
}

int genos::drivers::avr_usart::sendbyte(int symbol)
{
	return usart_regs_sendbyte(regs, symbol);
}

int genos::drivers::avr_usart::cantx()
{
	return usart_regs_cansend(regs);
}

int genos::drivers::avr_usart::hasrx()
{
	return usart_regs_canrecv(regs);
}

int genos::drivers::avr_usart::setup(int32_t baud,
                                     char parity,
                                     uint8_t databits,
                                     uint8_t stopbits)
{
	usart_regs_setup(regs, baud, parity, databits, stopbits);
	irqinit();
	return 0;
}

void genos::drivers::avr_usart::_rx_handler(void * arg)
{
	genos::drivers::avr_usart* usart = (genos::drivers::avr_usart*) arg;

	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_RX);
}

void genos::drivers::avr_usart::_dre_handler(void * arg)
{
	genos::drivers::avr_usart* usart = (genos::drivers::avr_usart*) arg;

	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_TX);
}

///Прерывания опустошения конвеера. Обычно не требуется.
void genos::drivers::avr_usart::_tx_handler(void * arg)
{
	genos::drivers::avr_usart* usart = (genos::drivers::avr_usart*) arg;

	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_TC);
}

int genos::drivers::avr_usart::irqinit()
{
	irqtable_set_handler(base_irq    , _rx_handler, (void*)this);
	irqtable_set_handler(base_irq + 1, _dre_handler, (void*)this);
	irqtable_set_handler(base_irq + 2, _tx_handler, (void*)this);

	return 0;
}