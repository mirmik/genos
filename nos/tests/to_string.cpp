#include <doctest/doctest.h>
#include <nos/to_string.h>

TEST_CASE("to_string")
{
    CHECK(nos::to_string(1) == "1");
    CHECK(nos::to_string(1u) == "1");
    CHECK(nos::to_string(1ul) == "1");
    CHECK(nos::to_string(1ull) == "1");
    CHECK(nos::to_string(1ll) == "1");

    CHECK(nos::to_string_join(std::vector<int>{1, 2, 3}, ",") == "1,2,3");
}