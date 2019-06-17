#ifndef UTIL_ARDUINO_COMPAT_H
#define UTIL_ARDUINO_COMPAT_H

#include <defs/arduino.h>
#include <drivers/gpio/pin.h>

static inline 
void digitalWrite(int num, int en) 
{
	PINOUT[num].set(en);
} 

static inline
uint8_t digitalRead(int num) 
{
	return PINOUT[num].get();
}

static inline 
void pinMode(int pin, uint8_t mode) 
{
	PINOUT[pin].mode(mode == OUTPUT ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
} 

static inline 
gpio_pin getPin(int pin) 
{
	return PINOUT[pin];
} 

static inline
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else    
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);        
    }
}

#endif