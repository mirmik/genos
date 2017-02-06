#include "hal/arch.h"
#include "kernel/diag.h"
#include "stdlib.h"

extern struct diag_ops terminal_diag;

void arch_init(void)
{
	diag_setup(&terminal_diag);
	diag_init();
};

void arch_idle(void)
{};

void arch_halt(void)
{
	exit(-1);
};

void arch_shutdown(arch_shutdown_mode_t mode)
{
	if (mode == ARCH_SHUTDOWN_MODE_HALT) arch_halt();
	if (mode == ARCH_SHUTDOWN_MODE_REBOOT) exit(0);
	if (mode == ARCH_SHUTDOWN_MODE_ABORT) exit(-1);
	exit(-1);
};