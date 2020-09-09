#include <drivers/serial/stm32_usart.h>
#include <igris/util/retype.h>

#include <asm/stm32_gpio.h>
#include <asm/stm32_usart.h>

void genos::stm32_usart::init_gpio(gpio_pin tx, gpio_pin rx, int af)
{
	stm32_rcc_enable_gpio(tx.gpio);
	stm32_rcc_enable_gpio(rx.gpio);

	gpio_settings(tx.gpio, tx.mask, GPIO_MODE_ALTERNATE);
	gpio_settings(rx.gpio, rx.mask, GPIO_MODE_ALTERNATE);

	stm32_gpio_set_alternate(tx.gpio, tx.mask, af);
	stm32_gpio_set_alternate(rx.gpio, rx.mask, af);	
}

int genos::stm32_usart::enable(int en)
{
	stm32_usart_enable(regs, en);
	return 0;
}

int genos::stm32_usart::ctrirqs(uint8_t cmd)
{	
	switch (cmd)
	{
		case UART_CTRIRQS_TXON:
			stm32_usart_txirq_enable(regs, true);
			return 0;

		case UART_CTRIRQS_TXOFF:
			stm32_usart_txirq_enable(regs, false);
			return 0;
		
		case UART_CTRIRQS_RXON:
			stm32_usart_rxirq_enable(regs, true);
			return 0;
		
		case UART_CTRIRQS_RXOFF:
			stm32_usart_rxirq_enable(regs, false);
			return 0;
		
		case UART_CTRIRQS_TCON:
			stm32_usart_tcirq_enable(regs, true);
			return 0;
		
		case UART_CTRIRQS_TCOFF:
			stm32_usart_tcirq_enable(regs, false);
			return 0;
	}

	return -1;

}

int genos::stm32_usart::recvbyte()
{
	return stm32_usart_getc(regs);

}

int genos::stm32_usart::sendbyte(int symbol)
{
	return stm32_usart_putc(regs, symbol);
}

int genos::stm32_usart::cantx()
{
	return stm32_usart_room(regs);
}

int genos::stm32_usart::hasrx()
{
	return stm32_usart_avail(regs);
}

static void _irqhandler(void* priv)
{
	genos::stm32_usart * dev = (genos::stm32_usart *) priv;
	USART_TypeDef * regs = dev->regs;

	if (stm32_rxirq_status(regs)){
		dev->handler(dev->handarg, UART_IRQCODE_RX);
	}

	else if (stm32_txirq_status(regs)){
		dev->handler(dev->handarg, UART_IRQCODE_TX);
	}

	else if (stm32_tcirq_status(regs)) 
	{
		dev->handler(dev->handarg, UART_IRQCODE_TC);		
	}

	else if (stm32_overrun_irq_status(regs)) 
	{
		stm32_drop_overrun_flag(regs);
		dev->handler(dev->handarg, UART_IRQCODE_RX_OVERRUN);	
	}

	else 
	{
		dpr("stm32: unh usart irq usart->ISR: ");
		dprhex(dev->regs->ISR);
	}
}

void genos::stm32_usart::irqinit()
{
	irqtable_set_handler(irqno, &_irqhandler, (void*) this);
}

int genos::stm32_usart::setup(int32_t baud, char parity,
                             uint8_t databits, uint8_t stopbits)
{
	//STM32 count parity bit as one of databit.
	if (parity != 'n')
		databits += 1;

	stm32_usart_setup(regs, baud, parity, databits, stopbits);
	irqinit();
	nvic_enable_irq(irqno);

	return 0;
}