#ifndef GENOS_NUCLEO_401_PINOUT_H
#define GENOS_NUCLEO_401_PINOUT_H

union
{
	struct
	{
		const struct gpio_pin PINOUT[] =
		{

		};

		const struct gpio_pin PINOUT_A[] =
		{
			{ GPIOA, 0 }, //A0
			{ GPIOA, 1 }, //A1
			{ GPIOA, 4 }, //A2
			{ GPIOB, 0 }, //A3
			{ GPIOC, 1 }, //A4
			{ GPIOC, 0 }, //A5
		};
	};
	const struct gpio_pin * PINOUT;
};

#endif