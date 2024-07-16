#include <doctest/doctest.h>
#include <ralgo/binary_hypercube.h>
#include <ralgo/cartesian_grid.h>

TEST_CASE("cartesian_grid")
{
    std::vector<std::vector<double>> grid({{1, 2, 3}, {4, 5}});

    {
        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {0.5, 0.5}),
                 (std::vector<size_t>{0, 0}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {1.5, 0.5}),
                 (std::vector<size_t>{1, 0}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {2.5, 0.5}),

                 (std::vector<size_t>{2, 0}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {3.5, 0.5}),

                 (std::vector<size_t>{3, 0}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {0.5, 4.5}),

                 (std::vector<size_t>{0, 1}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {1.5, 4.5}),

                 (std::vector<size_t>{1, 1}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {2.5, 4.5}),

                 (std::vector<size_t>{2, 1}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {3.5, 4.5}),

                 (std::vector<size_t>{3, 1}));

        CHECK_EQ(ralgo::number_of_cartesian_grid_interval_for_ndim_point(
                     grid, {0.5, 5.5}),

                 (std::vector<size_t>{0, 2}));
    }
}

TEST_CASE("cell_indices_hypercube_with_collapsed_bounds. dim == 1")
{
    {
        auto vertices =
            ralgo::cell_indices_hypercube_with_collapsed_bounds({0}, {2});
        CHECK_EQ(vertices.size(), 2);
        CHECK_EQ(vertices[0], (ralgo::vector<size_t>{0}));
        CHECK_EQ(vertices[1], (ralgo::vector<size_t>{0}));
    }

    {
        auto vertices =
            ralgo::cell_indices_hypercube_with_collapsed_bounds({1}, {2});
        CHECK_EQ(vertices.size(), 2);
        CHECK_EQ(vertices[0], (ralgo::vector<size_t>{0}));
        CHECK_EQ(vertices[1], (ralgo::vector<size_t>{1}));
    }

    {
        auto vertices =
            ralgo::cell_indices_hypercube_with_collapsed_bounds({2}, {2});
        CHECK_EQ(vertices.size(), 2);
        CHECK_EQ(vertices[0], (ralgo::vector<size_t>{1}));
        CHECK_EQ(vertices[1], (ralgo::vector<size_t>{1}));
    }
}

TEST_CASE("cell_indices_hypercube_with_collapsed_bounds. dim == 2")
{
    {
        auto vertices =
            ralgo::cell_indices_hypercube_with_collapsed_bounds({0, 0}, {2, 2});
        CHECK_EQ(vertices.size(), 4);
        CHECK_EQ(vertices[0], (ralgo::vector<size_t>{0, 0}));
        CHECK_EQ(vertices[1], (ralgo::vector<size_t>{0, 0}));
        CHECK_EQ(vertices[2], (ralgo::vector<size_t>{0, 0}));
        CHECK_EQ(vertices[3], (ralgo::vector<size_t>{0, 0}));
    }
    {
        auto vertices =
            ralgo::cell_indices_hypercube_with_collapsed_bounds({1, 1}, {2, 2});
        CHECK_EQ(vertices.size(), 4);
        CHECK_EQ(vertices[0], (ralgo::vector<size_t>{0, 0}));
        CHECK_EQ(vertices[1], (ralgo::vector<size_t>{0, 1}));
        CHECK_EQ(vertices[2], (ralgo::vector<size_t>{1, 0}));
        CHECK_EQ(vertices[3], (ralgo::vector<size_t>{1, 1}));
    }
    {
        auto vertices =
            ralgo::cell_indices_hypercube_with_collapsed_bounds({1, 2}, {2, 2});
        CHECK_EQ(vertices.size(), 4);
        CHECK_EQ(vertices[0], (ralgo::vector<size_t>{0, 1}));
        CHECK_EQ(vertices[1], (ralgo::vector<size_t>{0, 1}));
        CHECK_EQ(vertices[2], (ralgo::vector<size_t>{1, 1}));
        CHECK_EQ(vertices[3], (ralgo::vector<size_t>{1, 1}));
    }
}