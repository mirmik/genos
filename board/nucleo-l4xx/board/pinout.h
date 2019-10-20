#ifndef BOARD_NUCLEO_L4XX_PINOUT_H
#define BOARD_NUCLEO_L4XX_PINOUT_H

#include <stdlib.h>
#include <drivers/gpio/pin.h>

#if defined(BOARD_NUCLEO_L432KC)
const gpio_pin PINOUT[] =
{
	{GPIOA, 10}, //0
	{GPIOA, 9}, //1
	{GPIOA, 12}, //2
	{GPIOB, 0}, //3
	{GPIOB, 7}, //4
	{GPIOB, 6}, //5
	{GPIOB, 1}, //6
	{NULL, 0}, //7
	{NULL, 0}, //8
	{GPIOA, 8}, //9
	{GPIOA, 11}, //10
	{GPIOB, 5}, //11
	{GPIOB, 4}, //12
	{GPIOB, 3}, //13


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

#elif defined(BOARD_NUCLEO_L412RB)
#warning "PINOUT_D is undefined"
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