#include <assert.h>
#include <igris/sync/critical_context.h>

static volatile int __critical_context_level = 0;

void assert_critical_context(void)
{
    assert(__critical_context_level >= 0 && __critical_context_level < 20);
}

void critical_context_level_inc(void)
{
    __critical_context_level++;
    assert_critical_context();
}

void critical_context_level_dec(void)
{
    __critical_context_level--;
    assert_critical_context();
}

int critical_context_level(void)
{
    return __critical_context_level;
}
