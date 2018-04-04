#include <genos/hal/board.h>
#include <genos/hal/arch.h>

void board_init() {}

void emergency_stop() __attribute__((weak));
void emergency_stop() {};

void board_shutdown(arch_shutdown_mode_t mode) { 
	emergency_stop();

	switch(mode)
	{
		case ARCH_SHUTDOWN_MODE_HALT:
		break;
		case ARCH_SHUTDOWN_MODE_REBOOT:
		break;
		case ARCH_SHUTDOWN_MODE_ABORT:
		break;
	};

	arch_shutdown(mode);
}