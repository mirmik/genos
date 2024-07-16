#include <asm/irq.h>
#include <asm/startup.h>
#include <genos/displace.h>
#include <genos/schedee.h>
#include <igris/sync/syslock.h>

__attribute__((deprecated)) int __displace__()
{
    genos::schedee *sch = genos::current_schedee();
    sch->syslock_counter_save = syslock_counter();
    return sch->displace();
}

__attribute__((weak)) void __schedule__()
{
    while (1)
        ;
}

__attribute__((weak)) void __context_displace_vector__()
{
    RESET_STACK();
    irqs_enable();
    syslock_reset();
    while (1)
        __schedule__();
}
