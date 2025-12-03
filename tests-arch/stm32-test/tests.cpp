#include <nos/check.h>
#include <onboardtest/test.h>

TEST_CASE("test0")
{
    NOS_CHECK_EQ(1, 1);
}

TEST_CASE("test1")
{
    NOS_CHECK_EQ(1, 1);
}

TEST_CASE("test3")
{
    NOS_CHECK_EQ(1, 1);
}

TEST_CASE("test4")
{
    NOS_CHECK_EQ(23, 42 - 19);
}