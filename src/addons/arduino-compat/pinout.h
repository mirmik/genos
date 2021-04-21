#ifndef ARDUINO_PINOUT_H
#define ARDUINO_PINOUT_H

#ifdef BOARD_ARDUINO_MEGA
#include <addons/arduino-compat/arduino-mega.h>

#elif BOARD_ARDUINO_UNO
#include <addons/arduino-compat/arduino-uno.h>

#else
#include <board/pinout.h>
#endif

#endif