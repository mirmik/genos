#ifndef GENOS_BOARD_H
#define GENOS_BOARD_H

#include <hal/arch.h>
#include <curboard.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

void board_init();
void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));
void emergency_stop();

__END_DECLS

#endif