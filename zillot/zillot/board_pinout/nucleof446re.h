#ifndef ZILLOT_NUCLEO_F446_PINOUT_H
#define ZILLOT_NUCLEO_F446_PINOUT_H

#include <zillot/stm32/pin.h>

namespace zillot
{
    namespace nucleo_f446
    {
        using pin = zillot::stm32::pin;

        pin PINOUT[] = {
            {GPIOA, 1 << 3},  // D0
            {GPIOA, 1 << 2},  // D1
            {GPIOA, 1 << 10}, // D2
            {GPIOB, 1 << 3},  // D3
            {GPIOB, 1 << 5},  // D4
            {GPIOB, 1 << 4},  // D5
            {GPIOB, 1 << 10}, // D6
            {GPIOA, 1 << 8},  // D7
            {GPIOA, 1 << 9},  // D8
            {GPIOC, 1 << 7},  // D9
            {GPIOB, 1 << 6},  // D10
            {GPIOA, 1 << 7},  // D11
            {GPIOA, 1 << 6},  // D12
            {GPIOA, 1 << 5},  // D13
            {GPIOB, 1 << 9},  // D14
            {GPIOB, 1 << 8},  // D15
        };
    }
}

// pwm pins:
// d2 - PWM1/3
// d3 - PWM2/2
// d4 - PWM3/2
// d5 - PWM3/1
// d6 - PWM2/3
// d7 - PWM1/1
// d8 - PWM1/2
// d9 - PWM3/2
// d10 - PWM4/1
// d11 - PWM1/1N
// d12 - PWM3/1
// d13 - PWM2/1
// d14 - PWM2/2
// d15 - PWM2/1

#endif