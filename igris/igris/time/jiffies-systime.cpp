#include <cstdint>
#include <igris/sync/syslock.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>

#include <asm/systime.h> // for systime_lomax and systime_lopart

uint64_t jiffies_to_millis = 1;
uint64_t lopart_to_micros = 1;

#define FREQSHIFT 8
#define FREQSHIFT2 10

typedef struct jiffies_pair_s
{
    jiffies_t hi;
    jiffies_t lo;
} jiffies_pair_t;

static volatile jiffies_t __jiffies = 0;

jiffies_t jiffies()
{
    system_lock();
    int64_t ticks = __jiffies;
    system_unlock();

    return ticks;
}

void jiffies_increment()
{
    __jiffies++;
}

void systime_set_frequency(uint32_t freq)
{
    jiffies_to_millis = ((uint32_t)1000 << FREQSHIFT) / freq;
    lopart_to_micros = ((uint32_t)1000 << FREQSHIFT2) / systime_lomax();
}

void delay(int64_t d)
{
    auto n = igris::millis();
    auto f = n + d;

    while (f - igris::millis() > 0)
        ;
}

jiffies_t ms2jiffies(uint32_t ms)
{
    return (ms << FREQSHIFT) / jiffies_to_millis;
}

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
        // pass: unlock for jiffies increment
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

int jiffies_pair_compare(jiffies_pair_t a, jiffies_pair_t b)
{
    if (a.hi != b.hi)
        return a.hi > b.hi ? 1 : -1;
    if (a.lo != b.lo)
        return a.lo > b.lo ? 1 : -1;
    return 0;
}

int64_t igris::micros()
{
    jiffies_pair_t pair = jiffies_pair();
    return jiffies_pair_to_micros(pair);
}

int64_t igris::millis()
{
    return (jiffies() * jiffies_to_millis) >> FREQSHIFT;
}

void igris::delay(int64_t ms)
{
    auto start = igris::micros();
    auto interval = ms * 1000;

    while (true)
    {
        auto timestamp = igris::micros();
        if (timestamp - start >= interval)
        {
            break;
        }
    }

    return;
}

int64_t igris::system_time()
{
    return millis();
}