#include "doctest/doctest.h"

#include <compat/std/iterator_base_funcs.h>

#include <type_traits>

TEST_CASE("igris_std iterator distance and advance")
{
    int data[] = {5, 6, 7, 8, 9};

    auto first = data;
    auto last = data + 5;
    CHECK(igris_std::distance(first, last) == 5);

    igris_std::advance(first, 3);
    CHECK(*first == 8);

    igris_std::advance(first, -2);
    CHECK(*first == 6);
}

TEST_CASE("igris_std iterator traits")
{
    using traits = igris_std::iterator_traits<const int *>;
    CHECK(std::is_same<typename traits::value_type, int>::value);
    CHECK(std::is_same<typename traits::pointer, const int *>::value);
    CHECK(std::is_same<typename traits::reference, const int &>::value);

    igris_std::iterator<igris_std::forward_iterator_tag, int> it{};
    (void)it;
}
