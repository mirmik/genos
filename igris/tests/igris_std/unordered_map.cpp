#include "doctest/doctest.h"

#include <compat/std/unordered_map>
#include <compat/std/stdexcept>

TEST_CASE("igris_std::unordered_map basic insert and find")
{
    igris_std::unordered_map<int, int> map;

    auto first = map.emplace(1, 10);
    CHECK(first.second);
    CHECK(first.first->second == 10);

    auto second = map.insert({2, 20});
    CHECK(second.second);

    CHECK(map.size() == 2);
    CHECK_FALSE(map.empty());

    auto it = map.find(1);
    REQUIRE(it != map.end());
    CHECK(it->second == 10);

    const auto &const_ref = map;
    auto cit = const_ref.find(2);
    REQUIRE(cit != const_ref.end());
    CHECK(cit->second == 20);

    auto duplicate = map.insert({1, 30});
    CHECK_FALSE(duplicate.second);
    CHECK(map.size() == 2);
    CHECK(map.find(1)->second == 10);
}

TEST_CASE("igris_std::unordered_map operator bracket insertion")
{
    igris_std::unordered_map<int, int> map;

    map[5] = 100;
    CHECK(map.size() == 1);
    CHECK(map[5] == 100);

    map[5] = 42;
    CHECK(map.size() == 1);
    CHECK(map[5] == 42);

    map[6];
    CHECK(map.size() == 2);
    CHECK(map[6] == 0);
}

TEST_CASE("igris_std::unordered_map erase and rehash")
{
    igris_std::unordered_map<int, int> map;
    constexpr int count = 64;

    for (int i = 0; i < count; ++i)
    {
        auto res = map.emplace(i, i * i);
        REQUIRE(res.second);
    }

    CHECK(map.size() == count);

    for (int i = 0; i < count; i += 2)
    {
        CHECK(map.erase(i) == 1);
    }

    CHECK(map.size() == count / 2);

    for (int i = 1; i < count; i += 2)
    {
        auto it = map.find(i);
        REQUIRE(it != map.end());
        CHECK(it->second == i * i);
    }
}

TEST_CASE("igris_std::unordered_map iteration and clear")
{
    igris_std::unordered_map<int, int> map;
    map.emplace(10, 1);
    map.emplace(20, 2);
    map.emplace(30, 3);

    bool seen10 = false;
    bool seen20 = false;
    bool seen30 = false;
    int count = 0;

    for (auto &entry : map)
    {
        if (entry.first == 10)
            seen10 = true;
        else if (entry.first == 20)
            seen20 = true;
        else if (entry.first == 30)
            seen30 = true;
        ++count;
    }

    CHECK(count == map.size());
    CHECK(seen10);
    CHECK(seen20);
    CHECK(seen30);

    const auto &cref = map;
    int const_count = 0;
    for (auto it = cref.begin(); it != cref.end(); ++it)
    {
        ++const_count;
        CHECK(cref.find(it->first) != cref.end());
    }
    CHECK(const_count == map.size());

    map.clear();
    CHECK(map.empty());
    CHECK(map.begin() == map.end());
}

TEST_CASE("igris_std::unordered_map at throws when missing")
{
    igris_std::unordered_map<int, int> map;
    map.emplace(1, 5);

    CHECK(map.at(1) == 5);
    CHECK_THROWS_AS(map.at(2), igris_std::out_of_range);
}
