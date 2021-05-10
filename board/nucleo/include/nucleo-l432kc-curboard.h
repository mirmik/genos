#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <periph/map.h>
#include <stddef.h>

#include <stdlib.h>
#include <drivers/gpio/pin.h>

#define BOARD_HSE_FREQ 8000000

#define SYSLED_GPIO GPIOB
#define SYSLED_MASK (1<<3)
#define SYSLED_PIN 3
#define SYSUART LPUART1
#define SYSUART_IRQ LPUART1_IRQn
#define SYSUART_RX_GPIO GPIOA
#define SYSUART_TX_GPIO GPIOA
#define SYSUART_RX_PIN 15
#define SYSUART_TX_PIN 2
#define SYSUART_TX_AF 8
#define SYSUART_RX_AF 8

static
const struct gpio_pin PINOUT[] =
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

#endif