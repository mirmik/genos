#include <asm/irq.h>
#include <assert.h>
#include <igris/compiler.h>
#include <igris/sync/syslock.h>

static irqstate_t save;
static volatile int count = 0;

void system_lock()
{
    irqstate_t tmpsave = irqs_save();
    if (count == 0)
        save = tmpsave;
    ++count;
}

void system_unlock()
{
    --count;
    if (count == 0)
        irqs_restore(save);
}

void syslock_reset()
{
    count = 0;
}

int syslock_counter()
{
    return count;
}

void syslock_counter_set(int newcount)
{
    count = newcount;
}
