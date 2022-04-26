#include <drivers/serial/avr_usart.h>
#include <asm/avr_usart.h>

#include <igris/dprint.h>
#include <util/cpu_delay.h>


int avr_usart_device::enable(int en)
{
	avr_usart_enable_rx(regs, en);
	avr_usart_enable_tx(regs, en);
	avr_usart_tcirq(regs, false);
	avr_usart_txirq(regs, false);
	avr_usart_rxirq(regs, en);

	cpu_delay(10000);

	return 0;
}

int avr_usart_device::ctrirqs(uint8_t cmd)
{
	switch (cmd)
	{
		case UART_CTRIRQS_TXON:
			avr_usart_txirq(regs, true);
			return 0;

		case UART_CTRIRQS_TXOFF:
			avr_usart_txirq(regs, false);
			return 0;

		case UART_CTRIRQS_RXON:
			avr_usart_rxirq(regs, true);
			return 0;

		case UART_CTRIRQS_RXOFF:
			avr_usart_rxirq(regs, false);
			return 0;

		case UART_CTRIRQS_TCON:
			avr_usart_tcirq(regs, true);
			return 0;

		case UART_CTRIRQS_TCOFF:
			avr_usart_tcirq(regs, false);
			return 0;
	}

	return -1;
}

int avr_usart_device::recvbyte()
{
	return avr_usart_recvbyte(regs);
}

int avr_usart_device::sendbyte(int symbol)
{
	return avr_usart_sendbyte(regs, symbol);
}

int avr_usart_device::cantx()
{
	return avr_usart_cansend(regs);
}

int avr_usart_device::hasrx()
{
	return avr_usart_canrecv(regs);
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

#if defined DRIVERS_WITH_IRQTABLE 
int avr_usart_device::irqinit()
{
	irqtable_set_handler(base_irq    , _rx_handler, this);
	irqtable_set_handler(base_irq + 1, _dre_handler, this);
	irqtable_set_handler(base_irq + 2, _tx_handler, this);

	return 0;
}
#endif

int avr_usart_device::setup(int32_t baud,
                           char parity,
                           uint8_t databits,
                           uint8_t stopbits)
{
	avr_usart_enable_tx(regs, 1);
	avr_usart_enable_rx(regs, 1);

	avr_usart_setup(regs, baud, parity, databits, stopbits);
	avr_usart_rxirq(regs, true);

	return 0;
}