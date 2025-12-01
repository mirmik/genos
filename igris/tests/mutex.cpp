#include <atomic>
#include <chrono>
#include <thread>

#include <doctest/doctest.h>

#include <igris/sync/mutex.h>

TEST_CASE("mutex trylock basics")
{
    igris_mutex_t mtx;
    igris_mutex_init(&mtx);

    CHECK_EQ(igris_mutex_trylock(&mtx), 0);
    CHECK_EQ(igris_mutex_trylock(&mtx), -EBUSY);
    CHECK_EQ(igris_mutex_unlock(&mtx), 0);
    CHECK_EQ(igris_mutex_destroy(&mtx), 0);
}

TEST_CASE("mutex blocks and wakes waiters")
{
    igris_mutex_t mtx;
    igris_mutex_init(&mtx);

    std::atomic<int> state{0};
    std::thread first([&]() {
        igris_mutex_lock(&mtx);
        state.store(1, std::memory_order_release);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        state.store(2, std::memory_order_release);
        igris_mutex_unlock(&mtx);
    });

    while (state.load(std::memory_order_acquire) == 0)
        std::this_thread::yield();

    std::atomic<bool> waiter_acquired{false};
    std::thread second([&]() {
        igris_mutex_lock(&mtx);
        waiter_acquired.store(true, std::memory_order_release);
        igris_mutex_unlock(&mtx);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    CHECK_FALSE(waiter_acquired.load(std::memory_order_acquire));

    first.join();
    second.join();

    CHECK(waiter_acquired.load(std::memory_order_acquire));
    CHECK_EQ(igris_mutex_destroy(&mtx), 0);
}
