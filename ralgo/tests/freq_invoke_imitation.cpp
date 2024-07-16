#include <doctest/doctest.h>
#include <ralgo/freq_invoke_imitation.h>

using namespace std::chrono_literals;

TEST_CASE("freq_invoke_imitation")
{
    int a = 0;
    int b = 0;

    ralgo::freq_invoke_imitation imitator = {
        {100ms, [&a]() { a++; }},
        {10ms, [&b]() { b++; }},
    };

    imitator.timeskip(1s);
    CHECK_EQ(a, 10);
    CHECK_EQ(b, 100);
}