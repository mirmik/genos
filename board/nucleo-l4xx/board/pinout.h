#ifndef BOARD_NUCLEO_L4XX_PINOUT_H
#define BOARD_NUCLEO_L4XX_PINOUT_H

#include <stdlib.h>
#include <drivers/gpio/pin.h>

#if defined(BOARD_NUCLEO_L432KC)
const gpio_pin PINOUT[] =
{
	{GPIOA, 1 << 10}, //0
	{GPIOA, 1 << 9}, //1
	{GPIOA, 1 << 12}, //2
	{GPIOB, 1 << 0}, //3
	{GPIOB, 1 << 7}, //4
	{GPIOB, 1 << 6}, //5
	{GPIOB, 1 << 1}, //6
	{NULL, 0}, //7
	{NULL, 0}, //8
	{GPIOA, 1 << 8}, //9
	{GPIOA, 1 << 11}, //10
	{GPIOB, 1 << 5}, //11
	{GPIOB, 1 << 4}, //12
	{GPIOB, 1 << 3}, //13


	{GPIOA, 1 << 0}, //0 14
	{GPIOA, 1 << 1}, //1 15
	{GPIOA, 1 << 3}, //2 16
	{GPIOA, 1 << 4}, //3 17
	{GPIOA, 1 << 5}, //4 18
	{GPIOA, 1 << 6}, //5 19
	{GPIOA, 1 << 7}, //6 20
	{GPIOA, 1 << 2}, //7 21
};
#define NR_PINOUT_D 14
#define PINOUT_A (PINOUT + NR_PINOUT_D)

#elif defined(BOARD_NUCLEO_L412RB)
#warning "PINOUT_D is undefined"
const gpio_pin PINOUT[] =
{
	{GPIOA, 1 << 0}, //0
	{GPIOA, 1 << 1}, //1
	{GPIOA, 1 << 3}, //2
	{GPIOA, 1 << 4}, //3
	{GPIOA, 1 << 5}, //4
	{GPIOA, 1 << 6}, //5
	{GPIOA, 1 << 7}, //6
	{GPIOA, 1 << 2}, //7
};
#define NR_PINOUT_D 0
#define PINOUT_A (PINOUT + NR_PINOUT_D)

#endif

#endif