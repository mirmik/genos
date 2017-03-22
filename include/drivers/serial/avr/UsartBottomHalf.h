#ifndef AVR_USART_BOTTOM_HALF_H
#define AVR_USART_BOTTOM_HALF_H

#include <drivers/serial/avr/UsartDriver.h>
#include <utilxx/classes/ByteRing.h>

class AVRUsartBottomHalf : public AVRUsartDriver {
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

	virtual void transmitter_ring_empty_handler() = 0;
	virtual void received_char_handler(char c) = 0;

	AVRUsartBottomHalf(const usart_data& udata, gxx::buffer txbuf);

	friend void interruptHandler_UsartRX(AVRUsartBottomHalf* usart);
	friend void interruptHandler_UsartTX(AVRUsartBottomHalf* usart);
	friend void interruptHandler_UsartTC(AVRUsartBottomHalf* usart);
};

#endif