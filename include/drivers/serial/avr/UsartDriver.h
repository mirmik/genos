#ifndef AVR_UART_DRIVER_H
#define AVR_UART_DRIVER_H

#include <periph/usart.h>
#include <drivers/serial/Uart.h>

class AVRUsartDriver {
public:
	usart_regs* m_regs;
	uint8_t m_irqbase;

public:
	AVRUsartDriver(usart_regs* regs, int irqbase);

	int sendbyte(char c);
	int recvbyte();

	int cansend();
	int canrecv();

	int enable(bool en);

	int enableRX(bool en);
	int enableTX(bool en);

	int irqEnableRX(bool en);
	int irqEnableTX(bool en);
	int irqEnableTC(bool en);

	int setup(int32_t baud = 9600, 
		Uart::Parity parity = Uart::ParityNone, 
		Uart::StopBits stopBits = Uart::StopBitsOne, 
		Uart::DataBits dataBits = Uart::DataBitsEight);

	void setIRQHandlers(
		IRQHandler irqHandlerRX, void* argRX,
		IRQHandler irqHandlerTX, void* argTX, 
		IRQHandler irqHandlerTC, void* argTC
	);
};

#endif