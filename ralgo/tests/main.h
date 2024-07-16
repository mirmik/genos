#include <doctest/doctest.h>
#include <vector>

using dvec = std::vector<double>;
using dvec2 = std::vector<std::vector<double>>;
using bvec = std::vector<bool>;

#define LT_BEGIN_TEST(a, b) TEST_CASE(#b)
#define LT_END_TEST(a)
