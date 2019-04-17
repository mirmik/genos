#include <periph/map.h>
#include <asm/rcc.h>
#include <asm/usart.h>

uint32_t stm32_usart_declared_freq(struct usart_regs* regs) 
{
	switch ((uintptr_t)regs)
	{
		#ifdef USART1 
		case USART1_BASE: return stm32_declared_freq[DECLARED_FREQ_APB2_NO];
		#endif 

		#ifdef USART2 
		case USART2_BASE: return stm32_declared_freq[DECLARED_FREQ_APB1_NO];
		#endif 

		#ifdef USART6 
		case USART6_BASE: return stm32_declared_freq[DECLARED_FREQ_APB2_NO];
		#endif 
	};
	return 0;
}

int stm32_usart_putc(struct usart_regs * usart, char c) {
	usart->DR = c;
	return 1;
}

int stm32_usart_getc(struct usart_regs * usart) {
	return usart->DR;
}

int stm32_usart_room(struct usart_regs * usart) {
	return usart->SR & (1 << 7); //TXNE byte
}

int stm32_usart_avail(struct usart_regs * usart) {
	return usart->SR & (1 << 5); //RXNE byte
}