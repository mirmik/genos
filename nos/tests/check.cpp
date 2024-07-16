#include <doctest/doctest.h>
#include <nos/check.h>
#include <stdexcept>

TEST_CASE("check_env")
{
    int i = 0;
    NOS_CHECK_ENV_START()
    {
        NOS_CHECK(1 == 1);
        i++;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        throw std::runtime_error(nos_check_error_msg);
    }
    CHECK(i == 1);
}

TEST_CASE("check_env.fault")
{
    int i = 0;
    NOS_CHECK_ENV_START()
    {
        NOS_CHECK_EQ(1, 2);
        i++;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        i--;
    }
    CHECK(i == -1);
}

TEST_CASE("check_env.fault2")
{
    int i = 0;
    NOS_CHECK_ENV_START()
    {
        NOS_CHECK_EQ(3, 4);
        i++;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        i--;
    }
    CHECK(i == -1);
}

TEST_CASE("check_env.fault3")
{
    int i = 0;
    NOS_CHECK_ENV_START()
    {
        NOS_CHECK_EQ(4, 5);
        i++;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        i--;
    }
    CHECK(i == -1);
}

TEST_CASE("check_eq")
{
    int i = 0;
    NOS_CHECK_ENV_START()
    {
        NOS_CHECK_EQ(0, 1);
        i++;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        i--;
    }
    CHECK(i == -1);
}

TEST_CASE("check_eq2")
{
    int i = 0;
    NOS_CHECK_ENV_START()
    {
        NOS_CHECK_EQ(1, 2);
        i++;
    }
    NOS_CHECK_ENV_ERROR_HANDLER()
    {
        i--;
    }
    CHECK(i == -1);
}