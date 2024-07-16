#include <doctest/doctest.h>
#include <ralgo/linalg/vecops.h>

TEST_CASE("bound_to_box")
{
    {
        std::vector<double> box{1, 1, 1};
        std::vector<double> vec{2, 2, 2};
        auto res = ralgo::vecops::bound_to_box(vec, box);
        CHECK(res[0] == 1);
        CHECK(res[1] == 1);
        CHECK(res[2] == 1);
    }

    {
        std::vector<double> box{1, 1};
        std::vector<double> vec{2, 1};
        auto res = ralgo::vecops::bound_to_box(vec, box);
        CHECK(res[0] == 1);
        CHECK(res[1] == 0.5);
    }

    {
        std::vector<double> box{2, 2};
        std::vector<double> vec{2, 1};
        auto res = ralgo::vecops::ray_to_box(vec, box);
        CHECK(res[0] == 2);
        CHECK(res[1] == 1);
    }

    {
        std::vector<double> box{2, 2};
        std::vector<double> vec{2, 1};
        auto res = ralgo::vecops::bound_to_box(vec, box);
        CHECK(res[0] == 2);
        CHECK(res[1] == 1);
    }

    {
        std::vector<double> box{2, 2, 0};
        std::vector<double> vec{2, 1, 3};
        auto res = ralgo::vecops::bound_to_box(vec, box);
        CHECK(res[0] == 0);
        CHECK(res[1] == 0);
        CHECK(res[2] == 0);
    }

    {
        std::vector<double> box{2, 2, 0};
        std::vector<double> vec{2, 1, 0};
        auto res = ralgo::vecops::bound_to_box(vec, box);
        CHECK(res[0] == 2);
        CHECK(res[1] == 1);
        CHECK(res[2] == 0);
    }
}