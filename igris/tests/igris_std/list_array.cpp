#include "doctest/doctest.h"

#include <compat/std/array>
#include <compat/std/list>

#include <type_traits>

TEST_CASE("igris_std array exposes standard aliases")
{
    using array_type = igris_std::array<int, 3>;
    array_type arr{};
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    CHECK(std::is_same<typename array_type::value_type, int>::value);
    CHECK(std::is_same<typename array_type::pointer, int *>::value);
    CHECK(std::is_same<typename array_type::const_pointer, const int *>::value);

    CHECK(arr.front() == 1);
    CHECK(arr.back() == 3);
    CHECK(arr.size() == 3);
    CHECK_FALSE(arr.empty());

    arr.fill(7);
    CHECK(arr[0] == 7);
}

TEST_CASE("igris_std list basic operations")
{
    igris_std::list<int> values;
    CHECK(values.empty());

    values.push_back(2);
    values.push_front(1);
    values.push_back(3);

    CHECK(values.size() == 3);
    CHECK(values.front() == 1);
    CHECK(values.back() == 3);

    auto it = values.begin();
    CHECK(*it == 1);
    ++it;
    igris_std::list<int>::const_iterator cit = it;
    CHECK(*cit == 2);

    values.pop_front();
    CHECK(values.front() == 2);

    values.pop_back();
    CHECK(values.back() == 2);

    values.clear();
    CHECK(values.empty());
}
