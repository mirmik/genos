#include <main.h>
#include <ralgo/linalg/backpack.h>

using namespace ralgo;

#define EPSILON 1e-6

LT_BEGIN_TEST(ralgo_test_suite, backpack)
{
    double arr[12];
    matrix_view<double> mat(arr, 4, 3);
    matops::copy_from_cols(mat,
                           dvec2{{1, 0, 0, 1}, {0, 1, 0, -1}, {0, 0, 1, 0}});

    std::vector<double> res{0, 0, 0};
    std::vector<double> tgt{1, 1, 1, 0};

    svd_backpack(res, tgt, mat);

    CHECK_EQ(res[0], doctest::Approx(1));
    CHECK_EQ(res[1], doctest::Approx(1));
    CHECK_EQ(res[2], doctest::Approx(1));
}
LT_END_TEST(backpack)
