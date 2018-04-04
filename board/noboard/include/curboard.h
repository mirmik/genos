#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#include <sys/cdefs.h>
#include <genos/hal/arch.h>

__BEGIN_DECLS

void board_init();
void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));

void emergency_stop();

__END_DECLS

#endif