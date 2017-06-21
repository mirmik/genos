#ifndef GENOS_SERIAL_PORT_H
#define GENOS_SERIAL_PORT_H

#include <proto/stream.h>
#include <drivers/serial/UartParams.h>

#include <genos/sigslot/delegate.h>

class SerialPort : public Stream {
public:
	SerialDiscipline * discipline;

public:
	virtual int begin(int32_t baud = 9600, 
		Uart::Parity parity = Uart::ParityNone, 
		Uart::StopBits stopBits = Uart::StopBitsOne, 
		Uart::DataBits dataBits = Uart::DataBitsEight) = 0;

	virtual int end() = 0;
};

#endif