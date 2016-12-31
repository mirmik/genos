#ifndef GENOS_SERIAL_DRIVER_H
#define GENOS_SERIAL_DRIVER_H

#include <stdlib.h>
#include <kernel/file/file.h>

#include <utilxx/setget.h>
#include <utilxx/classes/Predicate.h>

class SerialDevice;

class SerialDriver : public FileAbstract {
public:
	//Predicate<SerialDriver*,char>* sigchecker;
	SerialDevice* ser;
public:
	SETTER(setDevice, ser);
	//virtual int write(const char* data, size_t size) = 0;
	//virtual int read(const char* data, size_t size) = 0;
	//virtual int flush() = 0;

	//virtual int avail() = 0;
	//virtual int room() = 0;

	virtual void rxHandler(char c) = 0;
	virtual void txHandler() = 0;
	virtual void tcHandler() = 0;

	//SerialDriver();
};

#endif