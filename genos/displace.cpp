#include <asm/irq.h>
#include <asm/startup.h>
#include <genos/displace.h>
#include <genos/schedee.h>
#include <igris/sync/syslock.h>

/*int __displace__()
{
    genos::schedee *sch = genos::current_schedee();

    if (sch->u.f.can_displace == 0)
        return -1;

    sch->syslock_counter_save = syslock_counter();

    //#if SCHEDEE_DEBUG_STRUCT
    //	++sch->dispcounter;
    //#endif

    // irqs_disable();
    return sch->displace();
}*/

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
