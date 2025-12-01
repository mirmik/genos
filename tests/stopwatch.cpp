#include <doctest/doctest.h>
#include <igris/time/stopwatch.h>

#include <chrono>
#include <thread>

TEST_CASE("stopwatch")
{
    igris::stopwatch sw;
    sw.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sw.stop();

    CHECK(sw.elapsed_ms() > 90);
    CHECK(sw.elapsed_ms() < 200);

    CHECK(sw.elapsed_us() > 90000);
    CHECK(sw.elapsed_us() < 200000);

    CHECK(sw.elapsed_ns() > 90000000);
    CHECK(sw.elapsed_ns() < 200000000);
}