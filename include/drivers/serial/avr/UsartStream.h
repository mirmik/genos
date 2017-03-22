#ifndef AVR_USART_STREAM_H
#define AVR_USART_STREAM_H

#include <drivers/serial/avr/UsartBottomHalf.h>
#include <utilxx/classes/ByteRing.h>

#include <proto/print.h>

class UsartStream : public AVRUsartBottomHalf, public Print {
private:
	ByteRing m_rxring;

public:
	UsartStream(const usart_data& udata, gxx::buffer txbuf, gxx::buffer rxbuf)
 		: AVRUsartBottomHalf(udata, txbuf), m_rxring(rxbuf) {}

	void transmitter_ring_empty_handler() {};
	void received_char_handler(char c) {};

	//int write(const char*, size_t);
	using AVRUsartBottomHalf::write;
};

#endif