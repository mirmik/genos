#ifndef STM32_UART_DRIVER_H
#define STM32_UART_DRIVER_H

#include <drivers/serial/Uart.h>
#include <periph/regs/usart.h>

//forward decl для возможности пофрендить сишную функцию.
class STM32HardwareUsart;
extern "C" void STM32HardwareUsart_interruptHandler(STM32HardwareUsart* usart);

class STM32HardwareUsart : public Uart {
private:
	usart_regs* m_regs;
	uint8_t m_irqno;

public:
	STM32HardwareUsart(usart_regs* regs, uint8_t irqno);

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

	friend void STM32HardwareUsart_interruptHandler(STM32HardwareUsart* usart);
};

#endif