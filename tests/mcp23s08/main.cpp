#include <hal/board.h>

#include <drivers/spi/stm32-spi.h>
#include "mcp23s08.h"

#include <Arduino.h>
#define SS 10
#define CHIP 0x40       // The chip's address (set by pins 4 & 5)
#define IO_DIR_REG 0x00 // The Input/Output Register
#define GPIO_REG 0x09   // The GPIO Register<br><br>void 


drivers::stm32::spi_driver spi2(SPI2, SPI2_IRQn);
mcp23s08 m1(&spi2, 9, 0x20);

void SPIWrite(uint8_t spiRegister, uint8_t value)
{
	uint8_t a, b, c;
	digitalWrite(SS, LOW); // 2) Send the chip's address to the chip
	a = spi2.exbyte(CHIP); // 3) Send the register to the chip
	b = spi2.exbyte(spiRegister); // 4) Send the value to the chip
	c = spi2.exbyte(value); //5) Pull the Slave/Chip select HIGH
	dprln(CHIP);
	dprln(spiRegister);
	dprln(value);
	dprln(a);
	dprln(b);
	dprln(c);
	digitalWrite(SS, HIGH);

	delay(1);  // Set pin 1 LOW<br>
	digitalWrite(SS, LOW); // 2) Send the chip's address to the chip

	dprln("Read");
	a = spi2.exbyte(CHIP + 1); // 3) Send the register to the chip
	b = spi2.exbyte(spiRegister); // 4) Send the value to the chip
	c = spi2.exbyte(0); //5) Pull the Slave/Chip select HIGH
	dprln(a);
	dprln(b);
	dprln(c);

	digitalWrite(SS, HIGH);
}

void loop();
int main()
{
	board_init();

	dprln("hello");

	irqs_enable();

	dprln("HERE");

	spi2.enable();
	m1.begin();
	pinMode(SS, OUTPUT);


	digitalWrite(SS, HIGH);
	delay(100);
	SPIWrite(IO_DIR_REG, 0x00); // Set all pins to OUTPUT
	SPIWrite(GPIO_REG, 0x00);  // Set all pins LOW

	while (1) loop();
}

void loop()
{
	// Set pin 1 HIGH
	SPIWrite(GPIO_REG, 0x01);
	delay(500);  // Set pin 1 LOW<br>
	SPIWrite(GPIO_REG, 0x00);
	delay(500);
}