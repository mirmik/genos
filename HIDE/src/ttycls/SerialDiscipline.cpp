#include <communication/ttycls/SerialDevice.h>

int StraightDiscipline::write(const char* data, size_t size) {
	return ser->drv->write(data, size);
}

int StraightDiscipline::read(char* data, size_t size) {
	return ser->drv->read(data, size);
}

int StraightDiscipline::flush() {
	return ser->drv->flush();
}

int StraightDiscipline::avail() {
	return ser->drv->avail();
}

int StraightDiscipline::room() {
	return ser->drv->room();
}
