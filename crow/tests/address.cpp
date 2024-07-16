#include <crow/address.h>
#include <doctest/doctest.h>

TEST_CASE("address")
{
    auto addr = crow::address("#F2.12.127.0.0.1:10009");
    auto res = crow::hostaddr(
        std::vector<uint8_t>{0xF2, 12, 127, 0, 0, 1, 0x27, 0x19});
    CHECK_EQ(addr, res);
}