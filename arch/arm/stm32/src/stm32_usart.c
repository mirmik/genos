#include <periph/map.h>
#include <asm/stm32_rcc.h>
#include <asm/stm32_usart.h>

#include <defs/uartdefs.h>

#ifdef USART_SR_RXNE
#define ISR SR
#define RDR DR
#define TDR DR
#define USART_ISR_RXNE USART_SR_RXNE
#define USART_ISR_TXNE USART_SR_TXNE
#define USART_ISR_TXE USART_SR_TXE
#define USART_ISR_TC USART_SR_TC
#endif

void stm32_usart_wait_cantx(USART_TypeDef * usart) 
{
	while (!(usart->ISR & (1 << 7)));
}

int stm32_usart_putc(USART_TypeDef * usart, char c)
{
	usart->TDR = c;
	return 1;
}

int stm32_usart_getc(USART_TypeDef * usart)
{
	return usart->RDR;
}

int stm32_usart_room(USART_TypeDef * usart)
{
	return usart->ISR & USART_ISR_TXE; 
}

int stm32_usart_avail(USART_TypeDef * usart)
{
	return usart->ISR & USART_ISR_RXNE;
}

int stm32_rxirq_status(USART_TypeDef * usart) 
{
	return usart->ISR & USART_ISR_RXNE;
}

int stm32_txirq_status(USART_TypeDef * usart) 
{
	return usart->ISR & USART_ISR_TXE;
}

int stm32_tcirq_status(USART_TypeDef * usart) 
{
	return usart->ISR & USART_ISR_TC;
}


uint8_t stm32_usart_clockbus(USART_TypeDef * regs)
{
	switch ((uintptr_t)regs)
	{
#ifdef USART1
		case USART1_BASE: return CLOCKBUS_APB2;
#endif

#ifdef USART2
		case USART2_BASE: return CLOCKBUS_APB1;
#endif

#ifdef USART6 
		case USART6_BASE: return CLOCKBUS_APB2;
#endif

		#ifdef RCC_APB1ENR2_LPUART1EN
		case LPUART1_BASE : return CLOCKBUS_APB1; break;
		#endif

		default: BUG();
	};
	return 0;
}

void stm32_usart_set_baudrate(USART_TypeDef * regs, uint32_t baud)
{
	uint32_t busfreq = stm32_clockbus_freq[stm32_usart_clockbus(regs)];

#ifdef LPUART1
	if (regs == LPUART1) 
		regs->BRR = ((double)(busfreq) / baud * 256);	
	else
		regs->BRR = busfreq / baud;
#else
	regs->BRR = busfreq / baud;
#endif	

}

void stm32_usart_set_parity(USART_TypeDef * regs, char parity)
{
	switch (parity)
	{
		case 'n':
			bits_clr(regs->CR1, 
				USART_CR1_PCE | USART_CR1_PS);
			break;

		case 'e':
			bits_assign(regs->CR1, 
				USART_CR1_PCE | USART_CR1_PS,
			            USART_CR1_PCE);
			break;

		case 'o':
			bits_assign(regs->CR1, 
				USART_CR1_PCE | USART_CR1_PS,
			    USART_CR1_PCE | USART_CR1_PS);
			break;

		default:
			BUG();
	}
	return;
}

// Note: If parity bit is using you should set one more databit.
// F.E. nine databits for eight information bits and one for parity.
void stm32_usart_set_databits(USART_TypeDef * regs, uint8_t databits)
{
	switch (databits)
	{
	#ifdef STM32F4XX
		case 8:
			bits_clr(regs->CR1, USART_CR1_M);
			break;

		case 9:
			bits_set(regs->CR1, USART_CR1_M);
			break;
	
	#elif defined(STM32G4XX)
		case 7:
			bits_clr(regs->CR1, USART_CR1_M0);
			bits_set(regs->CR1, USART_CR1_M1);
			break;

		case 8:
			bits_clr(regs->CR1, USART_CR1_M0);
			bits_clr(regs->CR1, USART_CR1_M1);
			break;

		case 9:
			bits_set(regs->CR1, USART_CR1_M0);
			bits_clr(regs->CR1, USART_CR1_M1);
			break;
	#else
	#error "undefined arch"
	#endif


		default:
			BUG();
	}
	return;
}

void stm32_usart_set_stopbits(USART_TypeDef * regs,
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

void stm32_usart_enable(USART_TypeDef * regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_UE)
	else
		bits_clr(regs->CR1, USART_CR1_UE);
}

void stm32_usart_enable_rx(USART_TypeDef * regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_RE)
	else
		bits_clr(regs->CR1, USART_CR1_RE);
}

void stm32_usart_enable_tx(USART_TypeDef * regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TE)
	else
		bits_clr(regs->CR1, USART_CR1_TE);
}

void stm32_usart_rxirq_enable(USART_TypeDef * regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_RXNEIE)
	else
		bits_clr(regs->CR1, USART_CR1_RXNEIE);
}

void stm32_usart_txirq_enable(USART_TypeDef * regs, int en)
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TXEIE)
	else
		bits_clr(regs->CR1, USART_CR1_TXEIE);
}

void stm32_usart_tcirq_enable(USART_TypeDef * regs, int en) 
{
	if (en)
		bits_set(regs->CR1, USART_CR1_TCIE)
	else
		bits_clr(regs->CR1, USART_CR1_TCIE);
}

int stm32_usart_setup(
    USART_TypeDef * regs,
    uint32_t baud,
    char parity,
    uint8_t databits,
    uint8_t stopbits)
{
	stm32_rcc_enable_usart(regs);

	stm32_usart_set_baudrate(regs, baud);
	stm32_usart_set_parity(regs, parity);
	stm32_usart_set_databits(regs, databits);
	stm32_usart_set_stopbits(regs, stopbits);

	regs->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; 

	return 0;
}

void stm32_usart_debug_print(USART_TypeDef * usart) 
{
	DPRINTHEX(usart->ISR);
	DPRINTHEX(usart->RDR);
	//DPRINTHEX(usart->BRR);
	DPRINTHEX(usart->CR1);
	DPRINTHEX(usart->CR2);
	DPRINTHEX(usart->CR3);
	//DPRINTHEX(usart->GTPR);
}