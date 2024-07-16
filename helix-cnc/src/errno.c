#include <asm/irq.h>
#include <errno.h>

static int errno__;

int *__errno_location() { return &errno__; }
int *__errno() { return &errno__; }

void emergency_halt()
{
    irqs_disable();
    while (1)
        ;
}