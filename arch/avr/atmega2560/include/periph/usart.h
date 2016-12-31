#ifndef AVR2560_USART_CLASS_H
#define AVR2560_USART_CLASS_H

#include <drivers/serial/Uart.h>
#include <periph/regs/usart.h>

class Usart : public Uart {
	
	volatile uart_regs *regs;

public:
	virtual void putc(char c) {}
	virtual char getc() {}
	
	virtual bool hasrx() {}
	virtual bool maytx() {}
	
	virtual void txEnable(bool) {}
	virtual void rxEnable(bool) {}

	virtual int setup(const Uart::Settings& param) {}

//	Usart(volatile uart_regs* addr) : regs(addr) {}
};

extern Usart usart0; 
extern Usart usart1; 
extern Usart usart2; 

#endif
