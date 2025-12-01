#include <cctype>
#include <cstdint>
#include <cstdio>
#include <doctest/doctest.h>
#include <igris/util/ctype.h>

TEST_CASE("ctype")
{
    for (uint16_t c = 0; c < 256; c++)
    {
        CHECK_EQ((uint8_t)(bool)isalpha(c), igris_isalpha(c));
        CHECK_EQ((uint8_t)(bool)isdigit(c), igris_isdigit(c));
        CHECK_EQ((uint8_t)(bool)isspace(c), igris_isspace(c));
        CHECK_EQ((uint8_t)(bool)isupper(c), igris_isupper(c));
        CHECK_EQ((uint8_t)(bool)islower(c), igris_islower(c));
        CHECK_EQ((uint8_t)(bool)isblank(c), igris_isblank(c));
        CHECK_EQ((uint8_t)(bool)isprint(c), igris_isprint(c));
        CHECK_EQ((uint8_t)(bool)isxdigit(c), igris_isxdigit(c));
    }
}