#ifndef AVR_USART_DEVICE_H
#define AVR_USART_DEVICE_H

#include <drivers/serial/Uart.h>
#include <drivers/serial/avr/UsartDriver.h>
#include <kernel/file/file.h>
#include <periph/usart.h>
#include <utilxx/classes/ByteRing.h>
#include <gxx/array.h>

class UsartDevice : public CharDevFile, public UsartDriver {

private:
	usart_regs* m_regs;
	uint8_t m_irqbase;

	ByteRing m_txring;
	ByteRing m_rxring;

public:
	int write(const char* data, size_t size);
	int read(char* data, size_t size);

	int getc();

	int flush();

	int avail();
	int room();

	int begin(int32_t baud = 9600, 
		Uart::Parity parity = Uart::ParityNone, 
		Uart::StopBits stopBits = Uart::StopBitsOne, 
		Uart::DataBits dataBits = Uart::DataBitsEight);
	
	int end();

	UsartDevice(const usart_data& udata, gxx::buffer txbuf, gxx::buffer rxbuf);

	friend void interruptHandler_UsartRX(UsartDevice* usart);
	friend void interruptHandler_UsartTX(UsartDevice* usart);
	friend void interruptHandler_UsartTC(UsartDevice* usart);
};

#endif