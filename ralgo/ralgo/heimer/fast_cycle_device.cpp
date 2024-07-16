#include <igris/util/bug.h>
#include <ralgo/heimer/fast_cycle_device.h>

DLIST_HEAD(heimer::fast_cycle_list);

__attribute__((weak)) double heimer::fast_cycle_frequence()
{
    return 1000;
}
