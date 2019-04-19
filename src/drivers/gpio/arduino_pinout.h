#ifndef ARDUINO_PINOUT_H
#define ARDUINO_PINOUT_H

#include <__local__/arch_gpio.h>
#include <drivers/gpio/pin.h>

#ifdef BOARD_ARDUINO_MEGA
const struct gpio_pin PINOUT[] = 
{
	{GPIOE, 1 << 0}, //0
	{GPIOE, 1 << 1}, //1
	{GPIOE, 1 << 4}, //2
	{GPIOE, 1 << 5}, //3
	{GPIOG, 1 << 5}, //4
	{GPIOE, 1 << 3}, //5
	{GPIOH, 1 << 3}, //6
	{GPIOH, 1 << 4}, //7
	{GPIOH, 1 << 5}, //8
	{GPIOH, 1 << 6}, //9
	{GPIOB, 1 << 4}, //10
	{GPIOB, 1 << 5}, //11 //OC1A
	{GPIOB, 1 << 6}, //12 //OC1B
	{GPIOB, 1 << 7}, //13 //OC1C
	{GPIOJ, 1 << 1}, //14
	{GPIOJ, 1 << 0}, //15
	{GPIOH, 1 << 1}, //16
	{GPIOH, 1 << 0}, //17
	{GPIOD, 1 << 3}, //18
	{GPIOD, 1 << 2}, //19
	{GPIOD, 1 << 1}, //20
	{GPIOD, 1 << 0}, //21
	{GPIOA, 1 << 0}, //22
	{GPIOA, 1 << 1}, //23
	{GPIOA, 1 << 2}, //24
	{GPIOA, 1 << 3}, //25
	{GPIOA, 1 << 4}, //26
	{GPIOA, 1 << 5}, //27
	{GPIOA, 1 << 6}, //28
	{GPIOA, 1 << 7}, //29
	{GPIOC, 1 << 7}, //30
	{GPIOC, 1 << 6}, //31
	{GPIOC, 1 << 5}, //32
	{GPIOC, 1 << 4}, //33
	{GPIOC, 1 << 3}, //34
	{GPIOC, 1 << 2}, //35
	{GPIOC, 1 << 1}, //36
	{GPIOC, 1 << 0}, //37
	{GPIOD, 1 << 7}, //38
	{GPIOG, 1 << 2}, //39
	{GPIOG, 1 << 1}, //40
	{GPIOG, 1 << 0}, //41
	{GPIOL, 1 << 7}, //42
	{GPIOL, 1 << 6}, //43
	{GPIOL, 1 << 5}, //44
	{GPIOL, 1 << 4}, //45
	{GPIOL, 1 << 3}, //46
	{GPIOL, 1 << 2}, //47
	{GPIOL, 1 << 1}, //48
	{GPIOL, 1 << 0}, //49
	{GPIOB, 1 << 3}, //50
	{GPIOB, 1 << 2}, //51
	{GPIOB, 1 << 1}, //52
	{GPIOB, 1 << 0}, //53
	{GPIOF, 1 << 0}, //54
	{GPIOF, 1 << 1}, //55
	{GPIOF, 1 << 2}, //56
	{GPIOF, 1 << 3}, //57
	{GPIOF, 1 << 4}, //58
	{GPIOF, 1 << 5}, //59
	{GPIOF, 1 << 6}, //60
	{GPIOF, 1 << 7}, //61
	{GPIOK, 1 << 0}, //62
	{GPIOK, 1 << 1}, //63
	{GPIOK, 1 << 2}, //64
	{GPIOK, 1 << 3}, //65
	{GPIOK, 1 << 4}, //66
	{GPIOK, 1 << 5}, //67
	{GPIOK, 1 << 6}, //68
	{GPIOK, 1 << 7}, //69
};
#else
#	error "Uncompatible board"
#endif


static inline void digitalWrite(int num, int lvl) 
{
	gpio_set_level(PINOUT[num].gpio, PINOUT[num].mask, lvl);
}

static inline int digitalRead(int num) 
{
	return gpio_get_level(PINOUT[num].gpio, PINOUT[num].mask);
}

static inline void pinMode(int num, int lvl) 
{
	if (lvl) gpio_settings(PINOUT[num].gpio, PINOUT[num].mask, GPIO_MODE_OUTPUT);
	else gpio_settings(PINOUT[num].gpio, PINOUT[num].mask, GPIO_MODE_INPUT);
}

#endif