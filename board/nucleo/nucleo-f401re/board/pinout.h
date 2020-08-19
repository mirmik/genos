#ifndef NUCLEOF401_BOARD_PINOUT_H
#define NUCLEOF401_BOARD_PINOUT_H

#include <drivers/gpio/pin.h>

static
const struct gpio_pin PINOUT[] = 
{
	//CN9
	{ GPIOA, 1<<3 },	//d0
	{ GPIOA, 1<<2 },	//d1
	{ GPIOA, 1<<10 },	//d2
	{ GPIOB, 1<<3 },	//d3
	{ GPIOB, 1<<5 },	//d4
	{ GPIOB, 1<<4 },	//d5
	{ GPIOB, 1<<10 },	//d6
	{ GPIOA, 1<<8 },	//d7

	//CN5
	{ GPIOA, 1<<9 },	//d8
	{ GPIOC, 1<<7 },	//d9
	{ GPIOB, 1<<6 },	//d10
	{ GPIOA, 1<<7 },	//d11
	{ GPIOA, 1<<6 },	//d12
	{ GPIOA, 1<<5 }, 	//d13
	{ GPIOB, 1<<9 },	//d14
	{ GPIOB, 1<<8 },	//d15
};


/*#ifdef BOARD_NUCLEO_L432

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
#endif*/

#endif