#include <drivers/serial/avr_usart.h>

#include <igris/dprint.h>
#include <hal/irqtable.h>
#include <hal/board.h>

#include <util/delay.h>

int avr_usart_device_enable(struct uart_device* u, int en)
{
	RETYPE(struct avr_usart_device *, dev, u);

	usart_regs_enable_rx(dev->regs, en);
	usart_regs_enable_tx(dev->regs, en);
	usart_regs_tcirq(dev->regs, false);
	usart_regs_txirq(dev->regs, false);
	usart_regs_rxirq(dev->regs, en);

	cpu_delay(10000);

	return 0;
}

int avr_usart_device_ctrirqs(struct uart_device* u, uint8_t cmd)
{
	RETYPE(struct avr_usart_device *, dev, u);

	switch (cmd)
	{
		case UART_CTRIRQS_TXON:
			usart_regs_txirq(dev->regs, true);
			return 0;

		case UART_CTRIRQS_TXOFF:
			usart_regs_txirq(dev->regs, false);
			return 0;

		case UART_CTRIRQS_RXON:
			usart_regs_rxirq(dev->regs, true);
			return 0;

		case UART_CTRIRQS_RXOFF:
			usart_regs_rxirq(dev->regs, false);
			return 0;

		case UART_CTRIRQS_TCON:
			usart_regs_tcirq(dev->regs, true);
			return 0;

		case UART_CTRIRQS_TCOFF:
			usart_regs_tcirq(dev->regs, false);
			return 0;
	}

	return -1;
}

int avr_usart_device_recvbyte(struct uart_device* u)
{
	RETYPE(struct avr_usart_device *, dev, u);
	return usart_regs_recvbyte(dev->regs);
}

int avr_usart_device_sendbyte(struct uart_device* u, int symbol)
{
	RETYPE(struct avr_usart_device *, dev, u);
	return usart_regs_sendbyte(dev->regs, symbol);
}

int avr_usart_device_cantx(struct uart_device* u)
{
	RETYPE(struct avr_usart_device *, dev, u);
	return usart_regs_cansend(dev->regs);
}

int avr_usart_device_hasrx(struct uart_device* u)
{
	RETYPE(struct avr_usart_device *, dev, u);
	return usart_regs_canrecv(dev->regs);
}

static void _rx_handler(void * arg)
{
	struct avr_usart_device* usart = (struct avr_usart_device*) arg;

	if (usart->dev.handler) usart->dev.handler(usart->dev.handarg, UART_IRQCODE_RX);
}

static void _dre_handler(void * arg)
{
	struct avr_usart_device* usart = (struct avr_usart_device*) arg;

	if (usart->dev.handler) usart->dev.handler(usart->dev.handarg, UART_IRQCODE_TX);
}

///Прерывания опустошения конвеера. Обычно не требуется.
static void _tx_handler(void * arg)
{
	struct avr_usart_device* usart = (struct avr_usart_device*) arg;

	if (usart->dev.handler) usart->dev.handler(usart->dev.handarg, UART_IRQCODE_TC);
}

int avr_usart_device_irqinit(struct uart_device* u)
{
	RETYPE(struct avr_usart_device *, dev, u);

	irqtable_set_handler(dev->base_irq    , _rx_handler, (void*)u);
	irqtable_set_handler(dev->base_irq + 1, _dre_handler, (void*)u);
	irqtable_set_handler(dev->base_irq + 2, _tx_handler, (void*)u);

	return 0;
}

int avr_usart_device_setup(struct uart_device* u, int32_t baud,
                           char parity,
                           uint8_t databits,
                           uint8_t stopbits)
{
	RETYPE(struct avr_usart_device *, dev, u);

	usart_regs_enable_tx(dev->regs, 1);
	usart_regs_enable_rx(dev->regs, 1);

	usart_regs_setup(dev->regs, baud, parity, databits, stopbits);
	avr_usart_device_irqinit(u);

	usart_regs_rxirq(dev->regs, true);

	return 0;
}

const struct uart_device_operations avr_usart_device_operations =
{
	.enable = avr_usart_device_enable,
	.ctrirqs = avr_usart_device_ctrirqs,
	.recvbyte = avr_usart_device_recvbyte,
	.sendbyte = avr_usart_device_sendbyte,
	.cantx = avr_usart_device_cantx,
	.hasrx = avr_usart_device_hasrx,
	.setup = avr_usart_device_setup
};