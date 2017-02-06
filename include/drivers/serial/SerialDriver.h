#ifndef GENOS_SERIAL_DRIVER_H
#define GENOS_SERIAL_DRIVER_H

#include <stdlib.h>
#include <kernel/file/file.h>

#include <utilxx/setget.h>
#include <utilxx/classes/Predicate.h>

/*
	SerialDriver it's abstract class to represent
	all Uart, Usart, or another serial line interface's drivers.

	implementations : UartRingDriver
*/

class SerialDriver : public DeviceFile {
public:
	//FileAbstract api:
	//virtual char getc();
	//virtual int write(const char* data, size_t size) = 0;
	//virtual int read(char* data, size_t size) = 0;
	//virtual int avail() = 0;
	//virtual int room() = 0;
	//virtual int flush() = 0;

	virtual void rxHandler(char c) = 0;
	virtual void txHandler() = 0;
	virtual void tcHandler() = 0;
};

#endif