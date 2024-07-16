#include <doctest/doctest.h>
#include <ralgo/signal/fft.h>

#include <nos/print.h>
#include <vector>

TEST_CASE("fft")
{
    std::vector<double> freqs;

    auto tim = ralgo::vecops::linspace(0., 12 * M_PI * 2., 1 << 8);
    auto sig = ralgo::vecops::sin(tim);

    auto ret = ralgo::signal::spectre(sig.data(), sig.size(), sig.size());

    freqs.resize((1 << 8) / 2);
    ralgo::signal::rfftfreq(freqs.data(), (1 << 8) / 2, (1.) / (1 << 8));
}
/*
TEST_CASE("fft.2")
{
    std::vector<double> freqs;

     auto tim = ralgo::vecops::linspace(0., 12 * M_PI * 2., 254);
     auto sig = ralgo::vecops::sin(tim);

     auto ret = ralgo::signal::rfft(sig);

     CHECK_EQ(ret.size(), 128);

     nos::println(ret);
}*/
/*
TEST_CASE("fft.2")
{
    std::vector<double> freqs;

     auto tim = ralgo::vecops::linspace(0., 12 * M_PI * 2., 100000);
     auto sig = ralgo::vecops::sin(tim);

     auto ret = ralgo::signal::rfft(sig);

     CHECK_EQ(ret.size(), 65536);
}
*/