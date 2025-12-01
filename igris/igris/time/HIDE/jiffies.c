#include <igris/sync/syslock.h>
#include <igris/time/jiffies_time.h>

volatile jiffies_t __jiffies = 0;

void system_tick() { ++__jiffies; }

jiffies_t jiffies()
{
    system_lock();
    jiffies_t ret = __jiffies;
    system_unlock();
    return ret;
}
