#ifndef GENOS_UART_RING_DRIVER_H
#define GENOS_UART_RING_DRIVER_H

#include <communication/ttycls/SerialDriver.h>
#include <utilxx/classes/ByteRing.h>

//UART/USART Driver
#include <drivers/serial/Uart.h>

class UartRingDriver : public SerialDriver {
public: 
	ByteRing txring;
	ByteRing rxring;
	
	Uart* m_uart;

public:
	UartRingDriver(Uart* uart, char* txbuffer, size_t txsize, char* rxbuffer, size_t rxsize);

	int write(const char* data, size_t size);
	int read(char* data, size_t size);
	int flush();

	int avail();
	int room();

	void rxHandler(char c);
	void txHandler();
	void tcHandler();

	void begin(UartParams* params);
};

#endif