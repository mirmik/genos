#include <doctest/doctest.h>
#include <igris/util/bits.h>

#ifndef _MSC_VER
TEST_CASE("igris_test_suite.bits")
{
    uint32_t a = 0b101101101101;
    bits_masked_assign_multimap(a, 0b0101, 0b011, 3);
    CHECK_EQ(bits_multimap(0b1011, 0b101, 3), 0b101000101101);
    CHECK_EQ(a, 0b101011101011);
}

TEST_CASE("igris_test_suite.bits2")
{
    uint32_t a = 0;
    bits_masked_assign_multimap(a, 1 << 1, 2, 4);
    CHECK_EQ(a, 2 << 4);
}

TEST_CASE("igris_test_suite.bits3")
{
    uint32_t a = 0;
    bits_masked_assign_multimap(a, 1 << 5, 2, 4);
    CHECK_EQ(a, 2 << (4 * 5));
}

#endif
