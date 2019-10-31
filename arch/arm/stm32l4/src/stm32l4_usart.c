#include <periph/map.h>
#include <asm/rcc.h>
#include <asm/usart.h>
#include <asm/arch.h>

#include <defs/uartdefs.h>
#include <igris/dprint.h>

uint8_t stm32l4_usart_clockbus(usart_regs_t* regs)
{
	switch ((uintptr_t)regs)
	{
#ifdef LPUART1
		case LPUART1_BASE: return CLOCKBUS_NO_APB2;
#endif

#ifdef USART1
		case USART1_BASE: return CLOCKBUS_NO_APB2;
#endif

#ifdef USART2
		case USART2_BASE: return CLOCKBUS_NO_APB1;
#endif

#ifdef USART3
		case USART3_BASE: return CLOCKBUS_NO_APB1;
#endif
#ifdef USART6
		case USART6_BASE: return CLOCKBUS_NO_APB2;
#endif
	};
	return 0;
}

void stm32l4_usart_set_baudrate(usart_regs_t* regs, uint32_t baud)
{
	uint32_t busfreq = stm32_declared_clockbus_freq[stm32l4_usart_clockbus(regs)];
	if (regs == LPUART1)
		regs->BRR = busfreq / baud * 256;
	else
		regs->BRR = busfreq / baud;
}

void stm32l4_usart_set_parity(usart_regs_t* regs, char parity)
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
void stm32l4_usart_set_databits(usart_regs_t* regs, uint8_t databits)
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

void stm32l4_usart_set_stopbits(usart_regs_t* regs,
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

void stm32l4_usart_enable(usart_regs_t* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_UE)
	else
		bits_clr(regs->CR1, USART_CR1_UE);
}

void stm32l4_usart_enable_rx(usart_regs_t* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_RE)
	else
		bits_clr(regs->CR1, USART_CR1_RE);
}

void stm32l4_usart_enable_tx(usart_regs_t* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TE)
	else
		bits_clr(regs->CR1, USART_CR1_TE);
}

void stm32l4_usart_rxirq_enable(usart_regs_t* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_RXNEIE)
	else
		bits_clr(regs->CR1, USART_CR1_RXNEIE);
}

void stm32l4_usart_txirq_enable(usart_regs_t* regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TXEIE)
	else
		bits_clr(regs->CR1, USART_CR1_TXEIE);
}

void stm32l4_usart_tcirq_enable(usart_regs_t* regs, int en) 
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TCIE)
	else
		bits_clr(regs->CR1, USART_CR1_TCIE);
}

int stm32l4_usart_setup(
    usart_regs_t* regs,
    uint32_t baud,
    char parity,
    uint8_t databits,
    uint8_t stopbits)
{
	stm32l4_rcc_enable_usart(regs);

	stm32l4_usart_set_baudrate(regs, baud);
	stm32l4_usart_set_parity(regs, parity);
	stm32l4_usart_set_databits(regs, databits);
	stm32l4_usart_set_stopbits(regs, stopbits);

	regs->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; 
	
	return 0;
}

void stm32l4_usart_debug_print(usart_regs_t * usart) 
{
	DPRINTHEX(usart->ISR);
	DPRINTHEX(usart->RDR);
	DPRINTHEX(usart->BRR);
	DPRINTHEX(usart->CR1);
	DPRINTHEX(usart->CR2);
	DPRINTHEX(usart->CR3);
	DPRINTHEX(usart->GTPR);
}