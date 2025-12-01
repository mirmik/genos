/*#include <doctest/doctest.h>
#include <igris/sync/semaphore.h>

#ifndef WIN32
TEST_CASE("semaphore")
{
    igris_sem_t sem;
    int val;
    igris_sem_init(&sem, 0, 1);

    igris_sem_wait(&sem);
    igris_sem_getvalue(&sem, &val);
    CHECK_EQ(val, 0);

    igris_sem_post(&sem);
    igris_sem_getvalue(&sem, &val);
    CHECK_EQ(val, 1);

    igris_sem_post(&sem);
    igris_sem_getvalue(&sem, &val);
    CHECK_EQ(val, 2);

    igris_sem_destroy(&sem);
}
#endif*/
