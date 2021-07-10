#include <igris/sync/syslock.h>
#include <genos/jiffies.h>

volatile jiffies_t __jiffies = 0;

jiffies_t jiffies()
{
    system_lock();
    jiffies_t ret = __jiffies;
    system_unlock();
    return ret;
}
