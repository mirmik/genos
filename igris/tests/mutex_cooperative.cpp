#include <vector>

#include <doctest/doctest.h>

#include <igris/sync/mutex.h>
#include <support/cooperative_threads_test.h>

struct mutex_test_context
{
    igris_mutex_t *mutex;
    int id;
    std::vector<int> *trace;
};

static void mutex_worker(void *arg)
{
    auto *ctx = static_cast<mutex_test_context *>(arg);

    for (int i = 0; i < 3; ++i)
    {
        igris_mutex_lock(ctx->mutex);
        ctx->trace->push_back(ctx->id);
        igris::test::coop::yield();
        igris_mutex_unlock(ctx->mutex);
        igris::test::coop::yield();
    }
}

TEST_CASE("mutex cooperative threads")
{
    igris_mutex_t mutex;
    igris_mutex_init(&mutex);

    std::vector<int> trace;
    mutex_test_context c1{&mutex, 1, &trace};
    mutex_test_context c2{&mutex, 2, &trace};

    igris::test::coop::reset();
    igris::test::coop::spawn(mutex_worker, &c1);
    igris::test::coop::spawn(mutex_worker, &c2);
    igris::test::coop::run();

    CHECK_EQ(trace.size(), 6);
    for (size_t i = 1; i < trace.size(); ++i)
    {
        CHECK_NE(trace[i], trace[i - 1]);
    }

    CHECK_EQ(igris_mutex_destroy(&mutex), 0);
}
