#ifndef AVR_UART_DRIVER_H
#define AVR_UART_DRIVER_H

#include <drivers/serial/Uart.h>
#include <periph/map.h>
#include <periph/irqdefs.h>

class AVRHardwareUsart;

class AVRHardwareUsart : public Uart {
public:
	usart_regs* m_regs;
	uint8_t m_irqbase;

public:
	AVRHardwareUsart(usart_regs* regs, int irqbase);

	int putc(char c);
	int getc();

	int avail();
	int room();

	int enable(bool en);

	int enableRX(bool en);
	int enableTX(bool en);

	int irqEnableRX(bool en);
	int irqEnableTX(bool en);
	int irqEnableTC(bool en);
	
	void* rxarg;
	void* txarg;
	void* tcarg;

	int setup(UartParams *params);

//	friend void STM32HardwareUsart_interruptHandler(STM32HardwareUsart* usart);
};

#endif