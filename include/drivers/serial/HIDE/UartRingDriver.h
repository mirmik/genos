#ifndef GENOS_UART_RING_DRIVER_H
#define GENOS_UART_RING_DRIVER_H
/*
#include <drivers/serial/SerialDriver.h>
#include <utilxx/classes/ByteRing.h>

//UART/USART Driver
#include <drivers/serial/Uart.h>

class UartRingDriver : public SerialDriver {
public: 
	ByteRing txring;
	ByteRing rxring;
	
	Uart* m_uart;

	WaitQueue txwq;
	WaitQueue rxwq;

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

	bool waitwrite(WaiterBasic* waiter);
	bool waitread(WaiterBasic* waiter);
};
*/
#endif