#ifndef ARDUINO_PINOUT_H
#define ARDUINO_PINOUT_H

//#include <__local__/arch_gpio.h>
#include <drivers/gpio/pin.h>

#ifdef BOARD_ARDUINO_MEGA
#include <addons/arduino-compat/arduino-mega.h>

#elif BOARD_ARDUINO_UNO
#include <addons/arduino-compat/arduino-uno.h>

//#elif BOARD_NUCLEO_F401RE
//#include <addons/arduino-compat/nucleo-f401.h>

#else
#include <board/pinout.h>
#endif

#endif