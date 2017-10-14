#ifndef HAL_ARCH_H_
#define HAL_ARCH_H_

#include <genos/compiler.h>
#include <arch/arch.h>

typedef enum {
	ARCH_SHUTDOWN_MODE_HALT,
	ARCH_SHUTDOWN_MODE_REBOOT,
	ARCH_SHUTDOWN_MODE_ABORT,
} arch_shutdown_mode_t;

__BEGIN_DECLS

void arch_init();

void _NORETURN arch_halt(void);
void _NORETURN arch_shutdown(arch_shutdown_mode_t mode);

void emergency_stop();

__END_DECLS

#endif /* HAL_ARCH_H_ */