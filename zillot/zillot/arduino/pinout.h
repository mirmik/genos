#ifndef ARDUINO_PINOUT_H
#define ARDUINO_PINOUT_H

#ifdef BOARD_ARDUINO_MEGA
#include <zillot/arduino/arduino-mega.h>

#elif BOARD_ARDUINO_UNO
#include <zillot/arduino/arduino-uno.h>

//#elif BOARD_NUCLEO_F401RE
//#include <addons/arduino-compat/nucleo-f401.h>
#endif

#endif