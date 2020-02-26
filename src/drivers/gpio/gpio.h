#ifndef DRIVERS_GPIO_H
#define DRIVERS_GPIO_H

#include <asm/gpio.h>

#include <stdint.h>
#include <sys/cdefs.h>
#include <defs/gpiodefs.h>

#define GPIO_MODE_IN_SECTION        0x000000FF
#define GPIO_MODE_OUT_SECTION       0x0000FF00
#define GPIO_MODE_INT_SECTION       0xFF000000

#ifndef GPIO_MODE_ALTERNATE
#define GPIO_MODE_ALTERNATE     	0x00010000
#endif

#ifndef GPIO_MODE_INPUT
#define GPIO_MODE_INPUT             0x00000001
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

__BEGIN_DECLS

//extern struct gpio *gpio_by_num(int num_port);

extern int gpio_settings(gpio_regs_t *gpio, unsigned int mask, uint32_t mode);
extern int gpio_mode_fast(gpio_regs_t *gpio, unsigned int mask, uint32_t mode);

extern void gpio_write(gpio_regs_t *gpio, unsigned int mask, uint8_t level);
extern unsigned int gpio_read(gpio_regs_t *gpio, unsigned int mask);

extern void gpio_toggle(gpio_regs_t *gpio, unsigned int mask);

//extern int gpio_pin_irq_attach(struct gpio *gpio, unsigned int mask,
//		irq_handler_t pin_handler, int mode, void *data);

//extern int gpio_pin_irq_detach(struct gpio *gpio, unsigned int mask,
//		irq_handler_t pin_handler, int mode);

#ifdef CHIP_STM32
void gpio_alternate(gpio_regs_t *gpio, unsigned int mask, uint8_t code); 
#endif

__END_DECLS

#endif
