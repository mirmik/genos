#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#include <compiler.h>
#include <hal/arch.h>

#include <drivers/gpio.h>

__BEGIN_DECLS

void board_init();
void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));

void emergency_stop();

static const struct gpio_pin_s __user_led = {GPIOA, 1<<5};

__END_DECLS

#define USER_LED (&__user_led)

#endif