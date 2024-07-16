#include <doctest/doctest.h>
#include <ralgo/rxsignal/rxpid.h>

TEST_CASE("rxintegral")
{
    // rxcpp collection from values
    std::vector<double> values = {1, 2, 3, 4, 5};
    auto source = ralgo::rxlist<double>(values);

    auto integral_test = ralgo::rxintegral<double>(source, 1);
    std::vector<int> v;
    integral_test.subscribe([&v](auto value) { v.push_back(value); });

    CHECK(v.size() == 5);
    CHECK(v[0] == 1);
    CHECK(v[1] == 3);
    CHECK(v[2] == 6);
    CHECK(v[3] == 10);
    CHECK(v[4] == 15);
}

TEST_CASE("rxpid")
{
    auto source = ralgo::rxlist<double>(std::vector<double>{1, 1, 1, 1, 1});

    SUBCASE("kp=1 ki=0 kd=0")
    {
        auto pid_test = ralgo::rxpid<double>(1, 0, 0, 0.01, source);
        std::vector<double> v;
        pid_test.subscribe([&v](auto value) { v.push_back(value); });

        CHECK(v.size() == 5);
        CHECK(v[0] == 1);
        CHECK(v[1] == 1);
        CHECK(v[2] == 1);
        CHECK(v[3] == 1);
        CHECK(v[4] == 1);
    }

    SUBCASE("kp=1 ki=1 kd=0")
    {
        auto pid_test = ralgo::rxpid<double>(1, 1, 0, 0.01, source);
        std::vector<double> v;
        pid_test.subscribe([&v](auto value) { v.push_back(value); });

        CHECK(v.size() == 5);
        CHECK(v[0] == 1.01);
        CHECK(v[1] == 1.02);
        CHECK(v[2] == 1.03);
        CHECK(v[3] == 1.04);
        CHECK(v[4] == 1.05);
    }
}