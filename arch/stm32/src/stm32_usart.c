#include <periph/map.h>
#include <asm/rcc.h>
#include <asm/usart.h>

#include <defs/uartdefs.h>

uint8_t stm32_usart_clockbus(struct usart_regs* regs)
{
	switch ((uintptr_t)regs)
	{
#ifdef USART1
		case USART1_BASE: return CLOCKBUS_NO_APB2;
#endif

#ifdef USART2
		case USART2_BASE: return CLOCKBUS_NO_APB1;
#endif

#ifdef USART6
		case USART6_BASE: return CLOCKBUS_NO_APB2;
#endif
	};
	return 0;
}

void stm32_usart_set_baudrate(struct usart_regs* regs, uint32_t baud)
{
	uint32_t busfreq = stm32_declared_clockbus_freq[stm32_usart_clockbus(regs)];
	regs->BRR = busfreq / baud;
}

void stm32_usart_set_parity(struct usart_regs* regs, char parity)
{
	switch (parity)
	{
		case 'n':
			bits_clr(regs->CR1, USART_CR1_PCE | USART_CR1_PS);
			break;

		case 'e':
			bits_assign(regs->CR1, USART_CR1_PCE | USART_CR1_PS,
			            USART_CR1_PCE);
			break;

		case 'o':
			bits_assign(regs->CR1, USART_CR1_PCE | USART_CR1_PS,
			            USART_CR1_PCE | USART_CR1_PS);
			break;

		default:
			BUG();
	}
	return;
}

// Note: If parity bit is using you should set one more databit.
// F.E. nine databits for eight information bits and one for parity.
void stm32_usart_set_databits(struct usart_regs* regs, uint8_t databits)
{
	switch (databits)
	{
		case 8:
			bits_clr(regs->CR1, USART_CR1_M);
			break;

		case 9:
			bits_set(regs->CR1, USART_CR1_M);
			break;

		default:
			BUG();
	}
	return;
}

void stm32_usart_set_stopbits(struct usart_regs* regs,
                              enum uart_stopbits_e stopbits)
{
	switch (stopbits)
	{
		case 1:
			bits_clr(regs->CR2, USART_CR2_STOP);
			break;

		case 2:
			bits_assign(regs->CR2, USART_CR2_STOP, USART_CR2_STOP_0);
			break;

		case UART_STOPBITS_HALF:
			bits_assign(regs->CR2, USART_CR2_STOP, USART_CR2_STOP_1);
			break;

		case UART_STOPBITS_ONE_AND_HALF:
			bits_set(regs->CR2, USART_CR2_STOP);
			break;

		default:
			BUG();
	};

	return;
}

void stm32_usart_enable(struct usart_regs* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_UE)
	else
		bits_clr(regs->CR1, USART_CR1_UE);
}

void stm32_usart_enable_rx(struct usart_regs* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_RE)
	else
		bits_clr(regs->CR1, USART_CR1_RE);
}

void stm32_usart_enable_tx(struct usart_regs* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TE)
	else
		bits_clr(regs->CR1, USART_CR1_TE);
}

int stm32_usart_setup(
    struct usart_regs* regs,
    uint32_t baud,
    char parity,
    uint8_t databits,
    uint8_t stopbits)
{
	rcc_enable_usart(regs);

	stm32_usart_set_baudrate(regs, baud);
	stm32_usart_set_parity(regs, parity);
	stm32_usart_set_databits(regs, databits);
	stm32_usart_set_stopbits(regs, stopbits);

	regs->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; 
}


int stm32_usart_putc(struct usart_regs * usart, char c)
{
	usart->DR = c;
	return 1;
}

int stm32_usart_getc(struct usart_regs * usart)
{
	return usart->DR;
}

int stm32_usart_room(struct usart_regs * usart)
{
	return usart->SR & (1 << 7); //TXNE byte
}

int stm32_usart_avail(struct usart_regs * usart)
{
	return usart->SR & (1 << 5); //RXNE byte
}