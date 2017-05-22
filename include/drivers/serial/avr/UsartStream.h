#ifndef AVR_USART_STREAM_H
#define AVR_USART_STREAM_H

#include <gxx/buffer.h>
#include <drivers/serial/avr/UsartDriver.h>
#include <utilxx/classes/ByteRing.h>
#include <kernel/devices/serial/SyncStream.h>

namespace Genos {

class AvrUsartStream : public AvrUsartDriver, public SyncStream {

private:
	ByteRing m_rxring;
	ByteRing m_txring;

public:
	int io_write(const char* data, size_t size);
	
	int read(char* data, size_t size);
	int getc();
	
	int avail();
	int room();

	int begin(int32_t baud = 9600, 
		Uart::Parity parity = Uart::ParityNone, 
		Uart::StopBits stopBits = Uart::StopBitsOne, 
		Uart::DataBits dataBits = Uart::DataBitsEight);
	
	int end();

	AvrUsartStream(const usart_data& udata, gxx::buffer txbuf, gxx::buffer rxbuf);

	friend void interruptHandler_UsartRX(AvrUsartStream* usart);
	friend void interruptHandler_UsartTX(AvrUsartStream* usart);
	friend void interruptHandler_UsartTC(AvrUsartStream* usart);
};


}

#endif