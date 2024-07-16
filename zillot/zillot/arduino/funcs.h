#ifndef GENOS_ARDUINO_COMPAT_FUNCTIONS_H
#define GENOS_ARDUINO_COMPAT_FUNCTIONS_H

#include <zillot/gpiodefs.h>
#include <zillot/irq.h>

#include <defs/arduino.h>

#include <igris/dprint.h>
#include <igris/time/systime.h>
#include <zillot/arduino/arduino_pin.h>
#include <zillot/arduino/pinout.h>

static inline struct arduino_pin *getPin(int pin)
{
    return (struct arduino_pin *)&PINOUT[pin];
}

static inline void digitalWrite(int num, int en)
{
    arduino_pin_set(getPin(num), en);
}

static inline uint8_t digitalRead(int num)
{
    return arduino_pin_get(getPin(num));
}

static inline void pinMode(int pin, uint8_t mode)
{
    struct arduino_pin *gpin = getPin(pin);
    arduino_pin_mode(gpin, mode == OUTPUT ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
}

static inline void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder,
                            uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

static inline void interrupts() { irqs_enable(); }

static inline void noInterrupts() { irqs_disable(); }

static inline void delay(int32_t arg) { igris::delay(arg); }

/*static inline void delayMicroseconds(int us)
{
    delay(us/1000 + 1);
}*/

#endif