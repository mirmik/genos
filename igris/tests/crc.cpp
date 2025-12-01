#include <doctest/doctest.h>
#include <igris/util/crc.h>

TEST_CASE("crc32_ciit") 
{
    int32_t ret = igris_crc32("HelloWorld", 10, 0);     
    CHECK_EQ(ret, 1114288986);
}