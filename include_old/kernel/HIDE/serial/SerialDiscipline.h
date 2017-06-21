#ifndef GENOS_SERIAL_DISCIPLINE_H
#define GENOS_SERIAL_DISCIPLINE_H

#include <stdlib.h>
#include <utilxx/setget.h>

class SerialDevice;

class SerialDiscipline {
public:
	SerialDevice* ser;
	
public:
	SETTER(setDevice, ser);
	
	virtual int write(const char* data, size_t size) = 0;
	virtual int read(char* data, size_t size) = 0;
	virtual int flush() = 0;

	virtual int avail() = 0;
	virtual int room() = 0;
};

class StraightDiscipline : public SerialDiscipline {
	int write(const char* data, size_t size);
	int read(char* data, size_t size);
	int flush();

	int avail();
	int room();	
};

//extern StraightDiscipline straightDiscipline;

#endif