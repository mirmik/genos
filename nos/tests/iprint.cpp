#include <cmath>
#include <doctest/doctest.h>
#include <nos/io/sstream.h>
#include <nos/print.h>
#include <random>

TEST_CASE("ibin")
{
    nos::stringstream ss;

    SUBCASE("u8")
    {
        uint8_t u8 = 0x1C;
        nos::print_to(ss, nos::ibin(u8));
        CHECK_EQ(ss.str(), "0b00011100");
    }

    SUBCASE("u16")
    {
        uint16_t u16 = 0x1CB4;
        nos::print_to(ss, nos::ibin(u16));
        CHECK_EQ(ss.str(), "0b0001110010110100");
    }
}

TEST_CASE("ibinmap")
{
    nos::stringstream ss;
    uint16_t u16 = 0b0000000000010100;
    nos::print_to(ss, nos::ibinmap(u16, 1));
    CHECK_EQ(ss.str(), "{4:1,2:1,}");
}

TEST_CASE("ihex")
{
    nos::stringstream ss;

    SUBCASE("u8")
    {
        uint8_t u8 = 0b10101100;
        nos::print_to(ss, nos::ihex(u8));
        CHECK_EQ(ss.str(), "0xAC");
    }

    SUBCASE("u16")
    {
        uint16_t u16 = 0b1010101010101100;
        nos::print_to(ss, nos::ihex(u16));
        CHECK_EQ(ss.str(), "0xAAAC");
    }
}