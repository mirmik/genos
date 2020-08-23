#ifndef NUCLEO_BOARD_CURBOARD_H
#define NUCLEO_BOARD_CURBOARD_H

#include <nucleo-boards-defs.h>

#if defined(BOARD_NUCLEO_F401RE)
#include <nucleo-f401re-curboard.h>
#include <nucleo-f401re-pinout.h>
#elif defined(BOARD_NUCLEO_F446RE)
#include <nucleo-f446re-curboard.h>
#include <nucleo-f446re-pinout.h>
#elif defined(BOARD_NUCLEO_G474RE)
#include <nucleo-g474re-curboard.h>
#include <nucleo-g474re-pinout.h>
#else
#error "undefined nucleo board"
#endif

#endif