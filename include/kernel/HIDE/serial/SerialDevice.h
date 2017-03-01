#ifndef GENOS_SERIAL_DEVICE_H
#define GENOS_SERIAL_DEVICE_H

#include <communication/ttycls/SerialDriver.h>
#include <communication/ttycls/SerialDiscipline.h>

#include <kernel/file/file.h>
#include <stdlib.h>

class SerialDevice : public FileAbstract {
public:
	SerialDriver* drv;
	SerialDiscipline* ldisc;

public:
	SETTER(setDriver, drv);
	SETTER(setDiscipline, ldisc);

	SerialDevice(SerialDriver* driver, SerialDiscipline* discipline);

	int write(const char* data, size_t size);
	int read(char* data, size_t size);
	int flush();

	int avail();
	int room();
};

#endif