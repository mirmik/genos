#include <drivers/serial/stm32_usart.h>
#include <igris/util/retype.h>

int stm32_usart_device_enable(struct uart_device * u, int en)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	stm32_usart_enable(dev->regs, en);
	return 0;
}

int stm32_usart_device_ctrirqs(struct uart_device * u, uint8_t cmd)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	
	switch (cmd)
	{
		case UART_CTRIRQS_TXON:
			stm32_usart_txirq_enable(dev->regs, true);
			return 0;

		case UART_CTRIRQS_TXOFF:
			stm32_usart_txirq_enable(dev->regs, false);
			return 0;
		
		case UART_CTRIRQS_RXON:
			stm32_usart_rxirq_enable(dev->regs, true);
			return 0;
		
		case UART_CTRIRQS_RXOFF:
			stm32_usart_rxirq_enable(dev->regs, false);
			return 0;
		
		case UART_CTRIRQS_TCON:
			stm32_usart_tcirq_enable(dev->regs, true);
			return 0;
		
		case UART_CTRIRQS_TCOFF:
			stm32_usart_tcirq_enable(dev->regs, false);
			return 0;
	}

	return -1;

}

int stm32_usart_device_recvbyte(struct uart_device * u)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	return stm32_usart_getc(dev->regs);

}

int stm32_usart_device_sendbyte(struct uart_device * u, int symbol)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	return stm32_usart_putc(dev->regs, symbol);
}

int stm32_usart_device_cantx(struct uart_device * u)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	return stm32_usart_room(dev->regs);
}

int stm32_usart_device_hasrx(struct uart_device * u)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	return stm32_usart_avail(dev->regs);
}

static void _irqhandler(void* priv)
{
	struct stm32_usart_device * dev = (struct stm32_usart_device *) priv;

	if (stm32_rxirq_status(dev->regs)){
		dev->dev.handler(dev->dev.handarg, UART_IRQCODE_RX);
	}

	else if (stm32_txirq_status(dev->regs)){
		dev->dev.handler(dev->dev.handarg, UART_IRQCODE_TX);
	}

	else if (stm32_tcirq_status(dev->regs)) {
		//dev->dev.handler(dev->dev.handarg, UART_IRQCODE_TC);
		BUG();
	}

	else
		BUG();
}

static inline void irqinit(struct stm32_usart_device * u)
{
	irqtable_set_handler(u->irqno, &_irqhandler, (void*) u);
}

int stm32_usart_device_setup(struct uart_device * u,
                             int32_t baud, char parity,
                             uint8_t databits, uint8_t stopbits)
{
	RETYPE(struct stm32_usart_device *, dev, u);
	//STM32 count parity bit as one of databit.
	if (parity != 'n')
		databits += 1;

	stm32_usart_setup(dev->regs, baud, parity, databits, stopbits);
	irqinit(dev);
	nvic_enable_irq(dev->irqno - STM32_IRQ_FIRST);

	return 0;
}

const struct uart_device_operations stm32_usart_device_operations =
{
	.enable = stm32_usart_device_enable,
	.ctrirqs = stm32_usart_device_ctrirqs,
	.recvbyte = stm32_usart_device_recvbyte,
	.sendbyte = stm32_usart_device_sendbyte,
	.cantx = stm32_usart_device_cantx,
	.hasrx = stm32_usart_device_hasrx,
	.setup = stm32_usart_device_setup
};