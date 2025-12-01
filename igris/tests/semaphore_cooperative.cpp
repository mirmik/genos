#include <vector>

#include <doctest/doctest.h>

#include <igris/sync/semaphore.h>
#include <support/cooperative_threads_test.h>

struct semaphore_consumer_context
{
    igris_sem_t *sem;
    int id;
    std::vector<int> *trace;
};

struct semaphore_producer_context
{
    igris_sem_t *sem;
};

static void semaphore_consumer(void *arg)
{
    auto *ctx = static_cast<semaphore_consumer_context *>(arg);
    igris_sem_wait(ctx->sem);
    ctx->trace->push_back(ctx->id);
    igris::test::coop::yield();
}

static void semaphore_producer(void *arg)
{
    auto *ctx = static_cast<semaphore_producer_context *>(arg);
    igris::test::coop::yield();
    igris_sem_post(ctx->sem);
    igris::test::coop::yield();
    igris_sem_post(ctx->sem);
}

TEST_CASE("semaphore cooperative wait/post")
{
    igris_sem_t sem;
    igris_sem_init(&sem, 0, 0);

    std::vector<int> trace;
    semaphore_consumer_context c1{&sem, 1, &trace};
    semaphore_consumer_context c2{&sem, 2, &trace};
    semaphore_producer_context prod{&sem};

    igris::test::coop::reset();
    igris::test::coop::spawn(semaphore_consumer, &c1);
    igris::test::coop::spawn(semaphore_consumer, &c2);
    igris::test::coop::spawn(semaphore_producer, &prod);
    igris::test::coop::run();

    REQUIRE(trace.size() == 2);
    CHECK(trace[0] == 1);
    CHECK(trace[1] == 2);
    CHECK_EQ(igris_sem_destroy(&sem), 0);
}
