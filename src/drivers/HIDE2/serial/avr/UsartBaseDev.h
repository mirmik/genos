#ifndef AVR_USART_BASEDEVICE_H
#define AVR_USART_BASEDEVICE_H

#include <drivers/serial/Uart.h>
#include <drivers/serial/avr/UsartDriver.h>

#include <drivers/serial/UsartTopHalfDev.h>

#include <periph/usart.h>
#include <utilxx/classes/ByteRing.h>
#include <genos/sigslot/delegate.h>

#include <proto/print.h>

class UsartBaseDev : public UsartDriver, public UsartTopHalfDev {

private:
	ByteRing m_txring;

public:
	int write(const char* data, size_t size);
	int room();

	int begin(int32_t baud = 9600, 
		Uart::Parity parity = Uart::ParityNone, 
		Uart::StopBits stopBits = Uart::StopBitsOne, 
		Uart::DataBits dataBits = Uart::DataBitsEight);
	
	int end();

	UsartBaseDev(const usart_data& udata, gxx::buffer txbuf);

	friend void interruptHandler_UsartRX(UsartBaseDev* usart);
	friend void interruptHandler_UsartTX(UsartBaseDev* usart);
	friend void interruptHandler_UsartTC(UsartBaseDev* usart);
};

#endif