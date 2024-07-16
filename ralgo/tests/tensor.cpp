#include <doctest/doctest.h>
#include <ralgo/linalg/extlinalg.h>

using namespace linalg::aliases;

TEST_CASE("antisymmetric_tensor")
{
    linalg::vec<float, 3> arr = {1, 2, 3};
    auto res = ralgo::antisymmetric_tensor(arr);
    CHECK(res ==
          linalg::mat<float, 3, 3>({{0, 3, -2}, {-3, 0, 1}, {2, -1, 0}}));
}

TEST_CASE("steiner_theorem")
{
    double mass = 1;
    double3 local_center = {0, 0, 1};
    double3 local_inertia = {1, 1, 1};
    ralgo::pose3<double> world_pose = {{0, 0, 0, 1}, {0, 0, 0}};
    double3 target_center = {0, 0, 0};

    auto res = ralgo::steiner(
        mass, local_inertia, local_center, world_pose, target_center);
    CHECK(res == linalg::mat<double, 3, 3>({{2, 0, 0}, {0, 2, 0}, {0, 0, 1}}));
}