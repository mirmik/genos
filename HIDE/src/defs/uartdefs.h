#ifndef GENOS_DEFS_UARTDEFS_H
#define GENOS_DEFS_UARTDEFS_H

enum uart_stopbits_e 
{	
	UART_STOPBITS_ONE = 1,
	UART_STOPBITS_TWO = 2,
	UART_STOPBITS_HALF = 10,		
	UART_STOPBITS_ONE_AND_HALF = 11
};

enum uart_parity_e 
{	
	UART_PARITY_NONE = 'n',
	UART_PARITY_EVEN = 'e',
	UART_PARITY_ODD = 'o'
};

#endif
