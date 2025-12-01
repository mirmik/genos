#include <array>
#include <doctest/doctest.h>
#include <igris/container/span.h>

TEST_CASE("span")
{
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    igris::span<int> spn(arr.data(), arr.size());
    igris::span<int> spn2(arr);

    CHECK(spn.size() == 5);
    CHECK(spn.size_bytes() == 5 * sizeof(int));
    CHECK(spn[0] == 1);
    CHECK(spn[1] == 2);
    CHECK(spn[2] == 3);
    CHECK(spn[3] == 4);
    CHECK(spn[4] == 5);

    auto it = spn.begin();
    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    ++it;
    CHECK(*it == 3);
    ++it;
    CHECK(*it == 4);
    ++it;
    CHECK(*it == 5);
    ++it;
    CHECK(it == spn.end());
}