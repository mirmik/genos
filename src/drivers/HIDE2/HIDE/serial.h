#ifndef GENOS_DRIVERS_SERIAL_PORT
#define GENOS_DRIVERS_SERIAL_PORT

#include <kernel/device/chardev.h>

class SerialPort : public kernel::chardev {

	enum Parity {
		ParityEven, ParityOdd, ParityNone
	};

	class Settings {
		uint32_t baud_rate;
		uint8_t parity;
		uint8_t n_stop;
		uint8_t n_bits;
	};

	virtual int16_t putc(char c) = 0;
	virtual int16_t getc(char c) = 0;
		
	virtual int16_t room() = 0;
	virtual int16_t avail() = 0;

	virtual void settings(Settings settings);
	virtual Settings settings();
	
};

#include <asm/periph/serial.h>

#endif