#ifndef BOARD_PINOUT_H
#define BOARD_PINOUT_H

#include <drivers/gpio/pin.h>

#ifdef BOARD_NUCLEO_L432

const gpio_pin PINOUT[] =
{
	{GPIOA, 0}, //0
	{GPIOA, 1}, //1
	{GPIOA, 3}, //2
	{GPIOA, 4}, //3
	{GPIOA, 5}, //4
	{GPIOA, 6}, //5
	{GPIOA, 7}, //6
	{GPIOA, 2}, //7
};
#define NR_PINOUT_D 0
#define PINOUT_A (PINOUT + NR_PINOUT_D)

#endif

#endif