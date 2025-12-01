#include <doctest/doctest.h>
#include <igris/container/flat_map.h>

#include <string>

TEST_CASE("flat_map")
{
    igris::flat_map<std::string, int> map;

    map["a"] = 1;
    map["b"] = 2;
    map["c"] = 3;
    map["d"] = 4;

    SUBCASE("find")
    {
        REQUIRE(map.find("a")->second == 1);
        REQUIRE(map.find("b")->second == 2);
        REQUIRE(map.find("c")->second == 3);
        REQUIRE(map.find("d")->second == 4);
    }

    SUBCASE("at")
    {
        REQUIRE(map.at("a") == 1);
        REQUIRE(map.at("b") == 2);
        REQUIRE(map.at("c") == 3);
        REQUIRE(map.at("d") == 4);
    }

    REQUIRE_THROWS_AS(map.at("e"), std::out_of_range);
    REQUIRE(map.find("e") == map.end());
    REQUIRE(map.size() == 4);
}