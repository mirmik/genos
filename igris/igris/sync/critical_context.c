#include <assert.h>
#include <igris/sync/critical_context.h>

static volatile int __critical_context_level = 0;

void assert_critical_context()
{
    assert(__critical_context_level >= 0 && __critical_context_level < 20);
}

void critical_context_level_inc()
{
    __critical_context_level++;
    assert_critical_context();
}

void critical_context_level_dec()
{
    __critical_context_level--;
    assert_critical_context();
}

int critical_context_level()
{
    return __critical_context_level;
}
