#ifndef GENOS_AVR_UART_RING_DRIVER_H
#define GENOS_AVR_UART_RING_DRIVER_H

#include <drivers/serial/UartRingDriver.h>
#include <drivers/serial/avr/UartDriver.h>

class HardwareUsartDevice : public BasicUartRing, public AVRHardwareUsart {
public: 
	void rxHandler(char c);
	void txHandler();
	void tcHandler();

	HardwareUsartDevice(const struct usart_data* halinfo, char* txbuffer, size_t txsize, char* rxbuffer, size_t rxsize) 
		: AVRHardwareUsart(halinfo->regs, halinfo->irqbase),
		BasicUartRing(txbuffer, txsize, rxbuffer, rxsize) {};
};

#endif