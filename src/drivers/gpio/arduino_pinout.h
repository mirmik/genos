#ifndef ARDUINO_PINOUT_H
#define ARDUINO_PINOUT_H

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
	{GPIOB, 1 << 5}, //11
	{GPIOB, 1 << 6}, //12
	{GPIOB, 1 << 7}, //13
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