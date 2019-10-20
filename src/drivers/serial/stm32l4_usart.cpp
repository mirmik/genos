#include <drivers/serial/stm32l4_usart.h>
#include <igris/util/retype.h>

int stm32l4_usart_device::enable(int en)
{
	stm32l4_usart_enable(regs, en);
	return 0;
}

int stm32l4_usart_device::ctrirqs(uint8_t cmd)
{	
	switch (cmd)
	{
		case UART_CTRIRQS_TXON:
			stm32l4_usart_txirq_enable(regs, true);
			return 0;

		case UART_CTRIRQS_TXOFF:
			stm32l4_usart_txirq_enable(regs, false);
			return 0;
		
		case UART_CTRIRQS_RXON:
			stm32l4_usart_rxirq_enable(regs, true);
			return 0;
		
		case UART_CTRIRQS_RXOFF:
			stm32l4_usart_rxirq_enable(regs, false);
			return 0;
		
		case UART_CTRIRQS_TCON:
			stm32l4_usart_tcirq_enable(regs, true);
			return 0;
		
		case UART_CTRIRQS_TCOFF:
			stm32l4_usart_tcirq_enable(regs, false);
			return 0;
	}

	return -1;

}

int stm32l4_usart_device::recvbyte()
{
	return stm32l4_usart_getc(regs);

}

int stm32l4_usart_device::sendbyte(int symbol)
{
	return stm32l4_usart_putc(regs, symbol);
}

int stm32l4_usart_device::cantx()
{
	return stm32l4_usart_room(regs);
}

int stm32l4_usart_device::hasrx()
{
	return stm32l4_usart_avail(regs);
}

static void _irqhandler(void* priv)
{
	struct stm32l4_usart_device * dev = (struct stm32l4_usart_device *) priv;
	USART_TypeDef * regs = dev->regs;

	if (stm32l4_rxirq_status(regs)){
		dev->handler(dev->handarg, UART_IRQCODE_RX);
	}

	else if (stm32l4_txirq_status(regs)){
		dev->handler(dev->handarg, UART_IRQCODE_TX);
	}

	else if (stm32l4_tcirq_status(regs)) {
		//dev.handler(dev.handarg, UART_IRQCODE_TC);
		stm32l4_usart_debug_print(regs);
		BUG();
	}
}

void stm32l4_usart_device::irqinit()
{
	irqtable_set_handler(irqno, &_irqhandler, (void*) this);
}

int stm32l4_usart_device::setup(int32_t baud, char parity,
                             uint8_t databits, uint8_t stopbits)
{
	//STM32 count parity bit as one of databit.
	if (parity != 'n')
		databits += 1;

	stm32l4_usart_setup(regs, baud, parity, databits, stopbits);
	irqinit();
	nvic_enable_irq(irqno);

	return 0;
}