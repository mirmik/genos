#ifndef DEFS_GPIO_H
#define DEFS_GPIO_H

/*
	В этот файл вынесены общие настройки большинства реализуаций gpio
	с этими настройками работает драйвер gpio_settings.

	Более тонкая настройка может производиться в asm заголовках.
*/

#define GPIO_MODE_IN_SECTION        0x000000FF
#define GPIO_MODE_OUT_SECTION       0x0000FF00
//#define GPIO_MODE_INT_SECTION       0xFF000000

#ifndef GPIO_MODE_INPUT
#define GPIO_MODE_INPUT             0x00000001
#endif

#ifndef GPIO_MODE_IN_NOPULL
#define GPIO_MODE_IN_NOPULL         (1 << 4)
#endif

#ifndef GPIO_MODE_IN_PULL_UP
#define GPIO_MODE_IN_PULL_UP        (1 << 1)
#endif

#ifndef GPIO_MODE_IN_PULL_DOWN
#define GPIO_MODE_IN_PULL_DOWN      (1 << 2)
#endif

#ifndef GPIO_MODE_IN_SCHMITT
#define GPIO_MODE_IN_SCHMITT        (1 << 3)
#endif

#ifndef GPIO_MODE_OUTPUT
#define GPIO_MODE_OUTPUT            0x00000100
#endif

#ifndef GPIO_MODE_OUT_PUSH_PULL
#define GPIO_MODE_OUT_PUSH_PULL     (1 << 15)
#endif

#ifndef GPIO_MODE_OUT_OPEN_DRAIN
#define GPIO_MODE_OUT_OPEN_DRAIN    (1 << 14)
#endif

#ifndef GPIO_MODE_ANALOG_INPUT
#define GPIO_MODE_ANALOG_INPUT      0x00020000
#endif

#ifndef GPIO_MODE_ALTERNATE
#define GPIO_MODE_ALTERNATE     	0x00010000
#endif


/*
#ifndef GPIO_MODE_IN_INT_EN
#define GPIO_MODE_IN_INT_EN         (1 << 31)
#endif

#ifndef GPIO_MODE_INT_MODE_RISING
#define GPIO_MODE_INT_MODE_RISING   (1 << 30)
#endif

#ifndef GPIO_MODE_INT_MODE_FALLING
#define GPIO_MODE_INT_MODE_FALLING  (1 << 29)
#endif

#ifndef GPIO_MODE_INT_MODE_LEVEL0
#define GPIO_MODE_INT_MODE_LEVEL0   (1 << 28)
#endif

#ifndef GPIO_MODE_INT_MODE_LEVEL1
#define GPIO_MODE_INT_MODE_LEVEL1   (1 << 27)
#endif

#ifndef GPIO_MODE_IN_INT_DIS
#define GPIO_MODE_IN_INT_DIS        (1 << 25)
#endif*/

#endif