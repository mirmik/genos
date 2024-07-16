#ifndef GENOS_ARDUINO_UNO_PINOUT_H
#define GENOS_ARDUINO_UNO_PINOUT_H

#include <zillot/arduino/arduino_pin.h>
#include <periph/map.h>

const struct arduino_pin PINOUT[] = 
{
    {GPIOD, 1 << 0}, // 0
    {GPIOD, 1 << 1}, // 1
    {GPIOD, 1 << 2}, // 2
    {GPIOD, 1 << 3}, // 3
    {GPIOD, 1 << 4}, // 4
    {GPIOD, 1 << 5}, // 5
    {GPIOD, 1 << 6}, // 6
    {GPIOD, 1 << 7}, // 7

    {GPIOB, 1 << 0}, // 8
    {GPIOB, 1 << 1}, // 9
    {GPIOB, 1 << 2}, // 10
    {GPIOB, 1 << 3}, // 11
    {GPIOB, 1 << 4}, // 12
    {GPIOB, 1 << 5}, // 13
};

#endif