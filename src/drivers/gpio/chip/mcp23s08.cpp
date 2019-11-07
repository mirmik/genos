#include <stdio.h>

#include <inttypes.h>

#include <Arduino.h>

#include "mcp23s08.h"
//#include <../SPI/SPI->h>//this chip needs SPI

mcp23s08::mcp23s08()
{
#if defined (SPI_HAS_TRANSACTION)
	_spiTransactionsSpeed = MAXSPISPEED;//set to max supported speed (in relation to chip and CPU)
#else
	_spiTransactionsSpeed = 0;
#endif
}

void mcp23s08::setSPIspeed(uint32_t spispeed)
{
#if defined (SPI_HAS_TRANSACTION)
	if (spispeed > 0)
	{
		if (spispeed > MAXSPISPEED)
		{
			_spiTransactionsSpeed = MAXSPISPEED;
		}
		else
		{
			_spiTransactionsSpeed = spispeed;
		}
	}
	else
	{
		_spiTransactionsSpeed = 0;//disable SPItransactons
	}
#else
	_spiTransactionsSpeed = 0;
#endif
}


//return 255 if the choosed pin has no INT, otherwise return INT number
//if there's support for SPI transactions it will use SPI->usingInterrupt(intNum);
//to prevent problems from interrupt
/*USE:
  int intNumber = mcp.getInterruptNumber(gpio_int_pin);
  if (intNumber < 255){
    attachInterrupt(intNumber, keypress, FALLING);//attack interrupt
  } else {
    Serial.println("sorry, pin has no INT capabilities!");
  }
 */

/*int mcp23s08::getInterruptNumber(uint8_t pin) {
	int intNum = digitalPinToInterrupt(pin);
	if (intNum != NOT_AN_INTERRUPT) {
		#if defined (SPI_HAS_TRANSACTION)
			SPI->usingInterrupt(intNum);
		#endif
		return intNum;
	}
	return 255;
}*/

mcp23s08::mcp23s08(drivers::spi_driver* SPI, const uint8_t csPin, const uint8_t haenAdrs)
	: SPI(SPI)
{
	_spiTransactionsSpeed = 0;
	postSetup(csPin, haenAdrs);
}

mcp23s08::mcp23s08(drivers::spi_driver* SPI, const uint8_t csPin, const uint8_t haenAdrs, uint32_t spispeed)
	: SPI(SPI)
{
	postSetup(csPin, haenAdrs, spispeed);
}


void mcp23s08::postSetup(const uint8_t csPin, const uint8_t haenAdrs, uint32_t spispeed)
{
//#if defined (SPI_HAS_TRANSACTION)
//	if (spispeed > 0) setSPIspeed(spispeed);
//#endif
	_cs = csPin;
	if (haenAdrs >= 0x20 && haenAdrs <= 0x23) //HAEN works between 0x20...0x23
	{
		_adrs = haenAdrs;
		_useHaen = 1;
	}
	else
	{
		_adrs = 0;
		_useHaen = 0;
	}
	_readCmd =  (_adrs << 1) | 1;
	_writeCmd = _adrs << 1;
	//setup register values for this chip
	IOCON = 	0x05;
	GPINTEN = 	0x02;
	IODIR = 	0x00;
	GPPU = 		0x06;
	GPIO = 		0x09;
	IPOL = 		0x01;
	DEFVAL = 	0x03;
	INTF = 		0x07;
	INTCAP = 	0x08;
	OLAT = 		0x0A;
	INTCON = 	0x04;
}

void mcp23s08::begin(bool protocolInitOverride)
{
	if (!protocolInitOverride)
	{
		SPI->begin();
		/*#if defined (SPI_HAS_TRANSACTION)
		if (_spiTransactionsSpeed == 0){//do not use SPItransactons
			SPI->setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
			SPI->setBitOrder(MSBFIRST);
			SPI->setDataMode(SPI_MODE0);
		}
		#else//do not use SPItransactons
		SPI->setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
		SPI->setBitOrder(MSBFIRST);
		SPI->setDataMode(SPI_MODE0);
		#endif*/
	}
	//pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);
	delay(100);
//	_useHaen == 1 ? writeuint8_t(IOCON, 0b00101000) : writeuint8_t(IOCON, 0b00100000);
	


	digitalWrite(_cs, LOW);
	delay(1);
	SPI->exbyte(0x40);
	SPI->exbyte(IOCON);
	SPI->exbyte(0b00101000);
	digitalWrite(_cs, HIGH);
	delay(1);

	_gpioDirection = 0xFF;//all in
	_gpioState = 0x00;//all low
}





uint8_t mcp23s08::readAddress(uint8_t addr)
{
	uint8_t low_uint8_t = 0x00;
//	dprln("startSend");

	uint8_t prebuf[] = { _writeCmd, addr };

	uint8_t buf[3];
	buf[0] = _readCmd;
	buf[1] = addr;
	buf[2] = 0;

	/*digitalWrite(_cs, LOW);
	SPI->exchange(&prebuf, &prebuf, sizeof(prebuf));
	digitalWrite(_cs, HIGH);

	delay(10);*/

	digitalWrite(_cs, LOW);
	delay(1);
	SPI->exchange(&buf, &buf, 3);
	delay(1);
	digitalWrite(_cs, HIGH);

	return buf[2];
}



void mcp23s08::gpioPinMode(uint8_t mode)
{
	if (mode == INPUT)
	{
		_gpioDirection = 0xFF;
	}
	else if (mode == OUTPUT)
	{
		_gpioDirection = 0x00;
		_gpioState = 0x00;
	}
	else
	{
		_gpioDirection = mode;
	}
	writeuint8_t(IODIR, _gpioDirection);
}

void mcp23s08::gpioPinMode(uint8_t pin, bool mode)
{
	if (pin < 8) //0...7
	{
		mode == INPUT ? _gpioDirection |= (1 << pin) : _gpioDirection &= ~(1 << pin);
		writeuint8_t(IODIR, _gpioDirection);
	}
}

void mcp23s08::gpioPort(uint8_t value)
{
	if (value == HIGH)
	{
		_gpioState = 0xFF;
	}
	else if (value == LOW)
	{
		_gpioState = 0x00;
	}
	else
	{
		_gpioState = value;
	}
	writeuint8_t(GPIO, _gpioState);
}


uint8_t mcp23s08::readGpioPort()
{
	return readAddress(GPIO);
}

uint8_t mcp23s08::readGpioPortFast()
{
	return _gpioState;
}

/*int mcp23s08::gpioDigitalReadFast(uint8_t pin){
	if (pin < 8){//0...7
		int temp = bitRead(_gpioState,pin);
		return temp;
	} else {
		return 0;
	}
}*/

void mcp23s08::portPullup(uint8_t data)
{
	if (data == HIGH)
	{
		_gpioState = 0xFF;
	}
	else if (data == LOW)
	{
		_gpioState = 0x00;
	}
	else
	{
		_gpioState = data;
	}
	writeuint8_t(GPPU, _gpioState);
}




void mcp23s08::gpioDigitalWrite(uint8_t pin, bool value)
{
	if (pin < 8) //0...7
	{
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
		writeuint8_t(GPIO, _gpioState);
	}
}

void mcp23s08::gpioDigitalWriteFast(uint8_t pin, bool value)
{
	if (pin < 8) //0...8
	{
		value == HIGH ? _gpioState |= (1 << pin) : _gpioState &= ~(1 << pin);
	}
}

void mcp23s08::gpioPortUpdate()
{
	writeuint8_t(GPIO, _gpioState);
}

int mcp23s08::gpioDigitalRead(uint8_t pin)
{
	if (pin < 8) return (int)(readAddress(GPIO) & 1 << pin);
	return 0;
}

uint8_t mcp23s08::gpioRegisterReaduint8_t(uint8_t reg)
{
	uint8_t data = 0;
	startSend(1);
	SPI->exbyte(reg);
	data = SPI->exbyte(0);
	endSend();
	return data;
}


void mcp23s08::gpioRegisterWriteuint8_t(uint8_t reg, uint8_t data)
{
	writeuint8_t(reg, (uint8_t)data);
}

/* ------------------------------ Low Level ----------------*/
void mcp23s08::startSend(bool mode)
{
//#if defined (SPI_HAS_TRANSACTION)
//	if (_spiTransactionsSpeed > 0) SPI->beginTransaction(SPISettings(_spiTransactionsSpeed, MSBFIRST, SPI_MODE0));
//#endif
//#if defined(__FASTWRITE)
//	digitalWriteFast(_cs, LOW);
//#else
	digitalWrite(_cs, LOW);
//#endif

	if (mode == 1)
	{
		//dprln("exbyte _readCmd", _readCmd);
		SPI->exbyte(_readCmd);
	}
	else
	{
		//dprln("exbyte _writeCmd", _writeCmd);
		SPI->exbyte(_writeCmd);
	}
}

void mcp23s08::endSend()
{
//#if defined(__FASTWRITE)
//	digitalWriteFast(_cs, HIGH);
//#else
	digitalWrite(_cs, HIGH);
//#endif
//#if defined (SPI_HAS_TRANSACTION)
//	if (_spiTransactionsSpeed > 0) SPI->endTransaction();
//#endif
}


void mcp23s08::writeuint8_t(uint8_t addr, uint8_t data)
{
	startSend(0);
	SPI->exbyte(addr);
	SPI->exbyte(data);
	endSend();
}


