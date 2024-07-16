#include <doctest/doctest.h>
#include <ralgo/linalg/gpssolver.h>

/*TEST_CASE("gpssolver")
{
    double T = 15;
    std::vector<linalg::vec<double, 4>> points = {
        linalg::vec<double, 4>{-1.0+1,  0.0+16,  0.0, T},
        linalg::vec<double, 4>{ 0.0+1, -1.0+16,  0.0, T},
        linalg::vec<double, 4>{ 0.0+1,  0.0+16, -1.0, T},
        linalg::vec<double, 4>{ 0.0+1,  0.0+16,  1.0, T},
        linalg::vec<double, 4>{ 0.0+1,  1.0+16,  0.0, T},
        linalg::vec<double, 4>{ 1./sqrt(2)+1,  1./sqrt(2)+16,  0.0, T},
    };

    auto x = ralgo::gps_solver(points, 4);

    CHECK_LT(std::abs(x[0] - 1), 1e-6);
    CHECK_LT(std::abs(x[1] - 16), 1e-6);
    CHECK_LT(std::abs(x[2] - 0), 1e-6);
    CHECK_LT(std::abs(x[3] - 15.25), 1e-6);
}*/