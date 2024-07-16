#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>
#include <map>
#include <string>
#include <ircc/ircc.h>

TEST_CASE("c_string")
{
    size_t size;
    const char *resource = ircc_c_string("another_key", &size);
    CHECK_EQ(size, 15);
    CHECK_EQ(strcmp("HelloUnderWorld", resource), 0);
}

TEST_CASE("string")
{
    std::string resource = ircc_string("another_key");
    CHECK_EQ(resource.size(), 15);
    CHECK_EQ("HelloUnderWorld", resource);
}

TEST_CASE("vector")
{
    const char *out = "HelloUnderWorld";
    std::vector<uint8_t> resource = ircc_vector("another_key");
    std::vector<uint8_t> outvec = {out, out + strlen(out)};
    CHECK_EQ(resource.size(), 15);
    CHECK_EQ(outvec, resource);
}

TEST_CASE("pair")
{
    auto [resource, size] = ircc_pair("another_key");
    CHECK_EQ(size, 15);
    CHECK_EQ(strcmp("HelloUnderWorld", resource), 0);
}

TEST_CASE("image")
{
    auto [resource, size] = ircc_pair("/image");
    CHECK_EQ(size, 38905);
}

TEST_CASE("find all")
{
    const char *resource;
    size_t size;

    SUBCASE("a")
    {
        auto [resource, size] = ircc_pair("/image");
    }
    SUBCASE("b")
    {
        auto [resource, size] = ircc_pair("/hello");
    }
    SUBCASE("c")
    {
        auto [resource, size] = ircc_pair("another_key");
    }

    CHECK_NE(resource, nullptr);
}

TEST_CASE("directory syntax")
{
    const char *resource;
    size_t size;

    SUBCASE("b")
    {
        auto [resource, size] = ircc_pair("/web/index.html");
    }
    SUBCASE("c")
    {
        auto [resource, size] = ircc_pair("/web/functions.json");
    }

    CHECK_NE(resource, nullptr);
}