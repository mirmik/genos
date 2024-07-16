#include <igris/time/systime.h>

void systime_set_frequency(uint32_t freq)
{
    system_frequency = freq;
    jiffies_to_millis = ((uint32_t)1000 << FREQSHIFT) / freq;
    lopart_to_micros = ((uint32_t)1000 << FREQSHIFT2) / systime_lomax();
}

jiffies_t millis() { return (jiffies() * jiffies_to_millis) >> FREQSHIFT; }

void delay(double d)
{
    jiffies_t n = millis();
    jiffies_t f = n + d;

    while (f - millis() > 0)
        ;
}

jiffies_t ms2jiffies(uint32_t ms)
{
    return (ms << FREQSHIFT) / jiffies_to_millis;
}

void system_tick() { ++__jiffies; }

jiffies_pair_t jiffies_pair()
{
    jiffies_pair_t res;
    jiffies_t lo1, hi2;

    do
    {
        system_lock();
        lo1 = systime_lopart();
        res.hi = __jiffies;
        res.lo = systime_lopart();
        system_unlock();

        system_lock();
        hi2 = __jiffies;
        system_unlock();
    } while (lo1 > res.lo || hi2 != res.hi);

    return res;
}

jiffies_t jiffies_pair_to_micros(jiffies_pair_t pair)
{
    jiffies_t res;

    res = ((pair.hi * jiffies_to_millis) >> FREQSHIFT) * 1000;
    res += (pair.lo * lopart_to_micros) >> FREQSHIFT2;

    return res;
}

jiffies_t micros()
{
    jiffies_pair_t pair = jiffies_pair();
    return jiffies_pair_to_micros(pair);
}

int jiffies_pair_compare(jiffies_pair_t a, jiffies_pair_t b)
{
    if (a.hi != b.hi)
        return a.hi > b.hi ? 1 : -1;
    if (a.lo != b.lo)
        return a.lo > b.lo ? 1 : -1;
    return 0;
}
