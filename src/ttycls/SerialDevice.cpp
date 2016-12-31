#include <communication/ttycls/SerialDevice.h>

SerialDevice::SerialDevice(SerialDriver* driver, SerialDiscipline* discipline) : drv(driver), ldisc(discipline) {
	drv->setDevice(this);
	ldisc->setDevice(this);
	//ldisc->setDevice(this);
}

int SerialDevice::write(const char* data, size_t size) {
	return ldisc->write(data, size);
}

int SerialDevice::read(char* data, size_t size) {
	return ldisc->read(data, size);
}

int SerialDevice::avail() {
	return ldisc->avail();
}

int SerialDevice::room() {
	return ldisc->room();
}

int SerialDevice::flush() {
	return ldisc->flush();
}
