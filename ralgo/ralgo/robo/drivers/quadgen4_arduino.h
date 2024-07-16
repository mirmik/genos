#ifndef RALGO_QUADGENER_H
#define RALGO_QUADGENER_H

#include <igris/compiler.h>
#include <igris/util/graycode.h>
#include <stdint.h>

#include <ralgo/robo/quadgen.h>

#if __has_include(<Arduino.h>)
#include <Arduino.h>
#else
// Затычка для того, чтобы скомпилировать функцию (для тестов)
// на машине отличной от Arduino
static inline void digitalWrite(uint8_t pin, uint8_t val)
{
    (void)pin;
    (void)val;
}
#endif

namespace robo
{
    class quadgen4_arduino : robo::quadgen
    {
        uint8_t apin0 = 0, apin1 = 0, bpin0 = 0, bpin1 = 0;

    public:
        quadgen4_arduino(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
        {
            apin0 = a;
            apin1 = b;
            bpin0 = c;
            bpin1 = d;
        }

        void apply_state(bool a, bool b) override
        {
            digitalWrite(apin0, a);
            digitalWrite(apin1, !a);
            digitalWrite(bpin0, b);
            digitalWrite(bpin1, !b);
        }
    };
}

#endif
