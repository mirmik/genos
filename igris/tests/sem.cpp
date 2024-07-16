/*#include <doctest/doctest.h>
#include <igris/sync/semaphore.h>

#ifndef WIN32
TEST_CASE("semaphore")
{
    sem_t sem;
    int val;
    sem_init(&sem, 0, 1);

    sem_wait(&sem);
    sem_getvalue(&sem, &val);
    CHECK_EQ(val, 0);

    sem_post(&sem);
    sem_getvalue(&sem, &val);
    CHECK_EQ(val, 1);

    sem_post(&sem);
    sem_getvalue(&sem, &val);
    CHECK_EQ(val, 2);

    sem_destroy(&sem);
}
#endif*/