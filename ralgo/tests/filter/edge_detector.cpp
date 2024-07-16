#include <doctest/doctest.h>
#include <ralgo/filter/edge_detector.h>

#include <ralgo/linalg/vecops.h>

TEST_CASE("edge_detector")
{
    ralgo::edge_detector detector(0.5);
    int acc_rising = 0;
    int acc_falling = 0;

    auto time = ralgo::vecops::linspace(0., 10. * 2 * M_PI, 1000);
    auto signal = ralgo::vecops::sin(time);

    for (auto &val : signal)
    {
        ralgo::EdgeDetectorStatus sig = detector.serve(val);
        acc_rising += sig == ralgo::EdgeDetectorStatus::RisingEvent ? 1 : 0;
        acc_falling += sig == ralgo::EdgeDetectorStatus::FallingEvent ? 1 : 0;
    }

    CHECK_EQ(acc_rising, 10);
    CHECK_EQ(acc_falling, 10);
}
