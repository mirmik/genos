#ifndef HAL_ARCH_H_
#define HAL_ARCH_H_

#include <igris/compiler.h> // For __NORETURN
#include <sys/cdefs.h>
//#include <asm/arch.h>

typedef enum {
	ARCH_SHUTDOWN_MODE_HALT,
	ARCH_SHUTDOWN_MODE_REBOOT,
	ARCH_SHUTDOWN_MODE_ABORT,
} arch_shutdown_mode_t;

__BEGIN_DECLS

void arch_init();

void __NORETURN arch_shutdown(arch_shutdown_mode_t mode);

static inline void arch_halt() { arch_shutdown(ARCH_SHUTDOWN_MODE_HALT); }
static inline void arch_reboot() { arch_shutdown(ARCH_SHUTDOWN_MODE_REBOOT); }
static inline void arch_abort() { arch_shutdown(ARCH_SHUTDOWN_MODE_ABORT); }

__END_DECLS

#endif /* HAL_ARCH_H_ */