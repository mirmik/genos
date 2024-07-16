#include <genos/ktimer.h>

void toggle_system_led();

genos::ktimer blink_timer(
    +[](void *arg, genos::ktimer *timer)
    {
        toggle_system_led();
        timer->replan();
    },
    nullptr,
    500);

void start_blink_schedee()
{
    blink_timer.plan();
}
