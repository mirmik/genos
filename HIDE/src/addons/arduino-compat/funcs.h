#ifndef GENOS_ARDUINO_COMPAT_FUNCTIONS_H
#define GENOS_ARDUINO_COMPAT_FUNCTIONS_H

#include <hal/irq.h>
#include <systime/systime.h>

#include <defs/arduino.h>
#include <addons/arduino-compat/pinout.h>

#include <igris/dprint.h>

static inline 
arduino_gpio_pin getPin(int pin) 
{
    return PINOUT[pin];
} 

static inline 
void digitalWrite(int num, int en) 
{
	getPin(num).set(en);
} 

static inline
uint8_t digitalRead(int num) 
{
	return getPin(num).get();
}

static inline 
void pinMode(int pin, uint8_t mode) 
{
    auto gpin = getPin(pin);
	gpin.mode(mode == OUTPUT ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
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

static inline void interrupts() 
{ irqs_enable(); }

static inline void noInterrupts() 
{ irqs_disable(); }

/*static inline void delayMicroseconds(int us) 
{
    delay(us/1000 + 1);
}*/

#endif