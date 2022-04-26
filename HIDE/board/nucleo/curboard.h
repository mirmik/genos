#ifndef NUCLEO_BOARD_CURBOARD_H
#define NUCLEO_BOARD_CURBOARD_H

#include <nucleo-boards-common.h>

#if defined(BOARD_NUCLEO_F401RE)
#include <nucleo-f401re-curboard.h>
#elif defined(BOARD_NUCLEO_F446RE)
#include <nucleo-f446re-curboard.h>
#include <nucleo-f446re-pinout.h>
#elif defined(BOARD_NUCLEO_G474RE)
#include <nucleo-g474re-curboard.h>
#include <nucleo-g474re-pinout.h>
#elif defined(BOARD_NUCLEO_G431RB)
#include <nucleo-g431rb.h>
#elif defined(BOARD_NUCLEO_L432KC)
#include <nucleo-l432kc-curboard.h>
#else
#error "undefined nucleo board"
#endif

#endif