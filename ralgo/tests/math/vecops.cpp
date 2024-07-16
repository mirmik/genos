#include <main.h>
#include <nos/print.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector.h>

using namespace ralgo;

LT_BEGIN_TEST(ralgo_test_suite, vecops)
{
    CHECK(vecops::all(bvec{true, true, true, false}) == false);
    CHECK(vecops::all(bvec{true, true, true, true}) == true);

    CHECK(vecops::any(bvec{true, false, false, false}) == true);
    CHECK(vecops::any(bvec{false, false, false, false}) == false);

    CHECK(vecops::equal_all(dvec{1, 2, 3}, dvec{1, 2, 3}));
    CHECK(vecops::equal_all(dvec{1, 2, 3}, dvec{1, 3, 4}) == false);
    CHECK(vecops::equal_any(dvec{1, 2, 3}, dvec{1, 3, 4}));
    CHECK(vecops::equal_any(dvec{1, 2, 3}, dvec{2, 3, 4}) == false);

    CHECK(vecops::equal_all(vecops::add_vv(dvec{1, 2, 3}, dvec{2, 3, 4}),
                            dvec{3, 5, 7}) == true);
    CHECK(vecops::equal_all(vecops::sub_vv(dvec{1, 2, 3}, dvec{2, 3, 5}),
                            dvec{-1, -1, -2}) == true);
}
LT_END_TEST(vecops)

TEST_CASE("vecops.copy_1")
{
    ralgo::vector<double> a = {1, 2, 3, 4, 5};
    std::vector<double> b = {0, 0, 0, 0, 0};
    vecops::copy(a, b);
    CHECK(vecops::equal_all(a, b));
}

TEST_CASE("vecops.copy_2")
{
    ralgo::vector<double> a = {1, 2, 3, 4, 5};
    auto b = vecops::copy<std::vector<double>>(a);
    CHECK(vecops::equal_all(a, b));
}