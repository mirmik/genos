#ifndef ZILLOT_ARDUINO_PIN_H
#define ZILLOT_ARDUINO_PIN_H

#include <asm/avr_gpio.h>

struct arduino_pin
{
    struct gpio_regs *regs;
    int mask;
};

static void arduino_pin_set(struct arduino_pin *pin, bool en)
{
    avr_gpio_set(pin->regs, pin->mask, en);
}

static int arduino_pin_get(struct arduino_pin *pin)
{
    return avr_gpio_get(pin->regs, pin->mask);
}

static int arduino_pin_mode(struct arduino_pin *pin, uint32_t m)
{
    return avr_gpio_mode(pin->regs, pin->mask, m);
}

#endif