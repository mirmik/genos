#include <drivers/serial/avr_usart.h>

#include <igris/dprint.h>
#include <hal/irqtable.h>
#include <hal/board.h>

#include <util/cpu_delay.h>

int avr_usart_device::enable(int en)
{
	usart_regs_enable_rx(regs, en);
	usart_regs_enable_tx(regs, en);
	usart_regs_tcirq(regs, false);
	usart_regs_txirq(regs, false);
	usart_regs_rxirq(regs, en);

	cpu_delay(10000);

	return 0;
}

int avr_usart_device::ctrirqs(uint8_t cmd)
{
	switch (cmd)
	{
		case UART_CTRIRQS_TXON:
			usart_regs_txirq(regs, true);
			return 0;

		case UART_CTRIRQS_TXOFF:
			usart_regs_txirq(regs, false);
			return 0;

		case UART_CTRIRQS_RXON:
			usart_regs_rxirq(regs, true);
			return 0;

		case UART_CTRIRQS_RXOFF:
			usart_regs_rxirq(regs, false);
			return 0;

		case UART_CTRIRQS_TCON:
			usart_regs_tcirq(regs, true);
			return 0;

		case UART_CTRIRQS_TCOFF:
			usart_regs_tcirq(regs, false);
			return 0;
	}

	return -1;
}

int avr_usart_device::recvbyte()
{
	return usart_regs_recvbyte(regs);
}

int avr_usart_device::sendbyte(int symbol)
{
	return usart_regs_sendbyte(regs, symbol);
}

int avr_usart_device::cantx()
{
	return usart_regs_cansend(regs);
}

int avr_usart_device::hasrx()
{
	return usart_regs_canrecv(regs);
}

static void _rx_handler(void * arg)
{
	struct avr_usart_device* usart = (struct avr_usart_device*) arg;

	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_RX);
}

static void _dre_handler(void * arg)
{
	struct avr_usart_device* usart = (struct avr_usart_device*) arg;

	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_TX);
}

///Прерывания опустошения конвеера. Обычно не требуется.
static void _tx_handler(void * arg)
{
	struct avr_usart_device* usart = (struct avr_usart_device*) arg;

	if (usart->handler) usart->handler(usart->handarg, UART_IRQCODE_TC);
}

int avr_usart_device::irqinit()
{
	irqtable_set_handler(base_irq    , _rx_handler, this);
	irqtable_set_handler(base_irq + 1, _dre_handler, this);
	irqtable_set_handler(base_irq + 2, _tx_handler, this);

	return 0;
}

int avr_usart_device::setup(int32_t baud,
                           char parity,
                           uint8_t databits,
                           uint8_t stopbits)
{
	usart_regs_enable_tx(regs, 1);
	usart_regs_enable_rx(regs, 1);

	usart_regs_setup(regs, baud, parity, databits, stopbits);
	irqinit();

	usart_regs_rxirq(regs, true);

	return 0;
}