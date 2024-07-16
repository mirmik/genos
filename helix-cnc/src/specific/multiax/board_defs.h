#ifndef BOARD_DEFS_H
#define BOARD_DEFS_H

#include <genos/chardev.h>
#include <zillot/common/uartring.h>

constexpr unsigned int TOTAL_CTRL_AXES = 5;

extern genos::zillot_chardev serial10_rs232;
extern genos::zillot_chardev serial8_console;

#define BOTTOM_LEVEL_PRESCALLER 100
#define BOTTOM_LEVEL_PERIOD 50
#define BOTTOM_LEVEL_TIMER_FREQ                                                \
    (stm32_clockbus_freq[CLOCKBUS_PLL] / 4) //  400 MHz clocking

#define SERVE_TIM TIM5
#define SERVE_TIM_IRQn TIM5_IRQn

#endif