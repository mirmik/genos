#include <doctest/doctest.h>
#include <ndmath/cartesian_correction.h>
#include <ndmath/util.h>
#include <nos/fprint.h>

TEST_CASE("cartesian_correction.perpendicular")
{
    ndmath::cartesian_correction cartesian_correction;

    cartesian_correction.set_grid({{0}, {-10, 0, 10}, {0}});
    CHECK_EQ(cartesian_correction.dim(), 3);
    cartesian_correction.set_deltas(igris::ndarray<ndmath::vector>(
        {{0, 0, 0.1}, {0, 0, 0.2}, {0, 0, 0.3}}, {1, 3, 1}));

    ndmath::vector point{0, 5, 0};
    auto corrected = cartesian_correction.corrected_point(point);

    CHECK_EQ(corrected, ralgo::vector<double>{0, 5, 0.25});
}

TEST_CASE("cartesian_correction.same")
{
    ndmath::cartesian_correction cartesian_correction;

    cartesian_correction.set_grid({{0}, {-10, 0, 10}, {0}});
    CHECK_EQ(cartesian_correction.dim(), 3);
    igris::ndarray<ndmath::vector> deltas(
        {ndmath::vector{0, 0, 0.1}, {0, 0, 0.2}, {0, 0, 0.3}}, {1, 3, 1});
    cartesian_correction.set_deltas(deltas);

    ndmath::vector point{0, 5, 0};
    auto corrected = cartesian_correction.corrected_point(point);

    CHECK_EQ(corrected, ralgo::vector<double>{0, 5, 0.25});
}

TEST_CASE("cartesian_correction.same center")
{
    ndmath::cartesian_correction cartesian_correction;

    cartesian_correction.set_grid({{0}, {-10, 0, 10}, {0}});
    CHECK_EQ(cartesian_correction.dim(), 3);
    igris::ndarray<ndmath::vector> deltas(
        {ndmath::vector{0, 0, 0.1}, {0, 0, 0.2}, {0, 0, 0.3}}, {1, 3, 1});
    cartesian_correction.set_deltas(deltas);

    ndmath::vector point{5, 0, 0};
    auto corrected = cartesian_correction.corrected_point(point);

    CHECK_EQ(corrected, ralgo::vector<double>{5, 0, 0.2});
}

TEST_CASE("cartesian_correction.same side")
{
    ndmath::cartesian_correction cartesian_correction;

    cartesian_correction.set_grid({{0}, {-10, 0, 10}, {0}});
    CHECK_EQ(cartesian_correction.dim(), 3);
    igris::ndarray<ndmath::vector> deltas(
        {ndmath::vector{0, 0, 0.1}, {0, 0, 0.0}, {0, 0, 0.5}}, {1, 3, 1});
    cartesian_correction.set_deltas(deltas);

    ndmath::vector point{0, 7.5, 0};
    auto corrected = cartesian_correction.corrected_point(point);

    CHECK_EQ(corrected[0], 0);
    CHECK_EQ(corrected[1], 7.5);
    CHECK_EQ(corrected[2], 0.5 * 3 / 4);
}

TEST_CASE("binary_hypercube")
{
    auto ndindexes = ralgo::binary_hypercube_vertices(3);
    CHECK_EQ(ndindexes[0], ralgo::vector<size_t>{0, 0, 0});
    CHECK_EQ(ndindexes[1], ralgo::vector<size_t>{0, 0, 1});
    CHECK_EQ(ndindexes[2], ralgo::vector<size_t>{0, 1, 0});
    CHECK_EQ(ndindexes[3], ralgo::vector<size_t>{0, 1, 1});
    CHECK_EQ(ndindexes[4], ralgo::vector<size_t>{1, 0, 0});
    CHECK_EQ(ndindexes[5], ralgo::vector<size_t>{1, 0, 1});
    CHECK_EQ(ndindexes[6], ralgo::vector<size_t>{1, 1, 0});
    CHECK_EQ(ndindexes[7], ralgo::vector<size_t>{1, 1, 1});
}

TEST_CASE("binary_hypercube")
{
    auto ndindexes = ralgo::binary_hypercube_vertices(1);
    CHECK_EQ(ndindexes[0], ralgo::vector<size_t>{0});
    CHECK_EQ(ndindexes[1], ralgo::vector<size_t>{1});
}

TEST_CASE("multidim_cell_indices")
{
    auto ndindexes = ralgo::cell_indices_hypercube_with_collapsed_bounds(
        {4, 7, 2}, {5, 8, 3});
    CHECK_EQ(ndindexes[0], ralgo::vector<size_t>{3, 6, 1});
    CHECK_EQ(ndindexes[1], ralgo::vector<size_t>{3, 6, 2});
    CHECK_EQ(ndindexes[2], ralgo::vector<size_t>{3, 7, 1});
    CHECK_EQ(ndindexes[3], ralgo::vector<size_t>{3, 7, 2});
    CHECK_EQ(ndindexes[4], ralgo::vector<size_t>{4, 6, 1});
    CHECK_EQ(ndindexes[5], ralgo::vector<size_t>{4, 6, 2});
    CHECK_EQ(ndindexes[6], ralgo::vector<size_t>{4, 7, 1});
    CHECK_EQ(ndindexes[7], ralgo::vector<size_t>{4, 7, 2});
}

TEST_CASE("cartesian_correction")
{
    ndmath::cartesian_correction cartesian_correction;

    cartesian_correction.set_grid({{-10, 0, 10}, {-20, 0, 20}, {-30, 0, 30}});

    cartesian_correction.set_deltas(
        igris::ndarray<ndmath::vector>{{
                                           {ndmath::vector{
                                                -1,
                                                1,
                                                0,
                                            },
                                            ndmath::vector{0, 1, 0},
                                            ndmath::vector{1, 1, 0}},
                                           {ndmath::vector{
                                                -1,
                                                0,
                                                0,
                                            },
                                            ndmath::vector{0, 0, 0},
                                            ndmath::vector{1, 0, 0}},
                                           {ndmath::vector{
                                                -1,
                                                -1,
                                                0,
                                            },
                                            ndmath::vector{0, -1, 0},
                                            ndmath::vector{1, -1, 0}},
                                       },
                                       {
                                           {ndmath::vector{
                                                -1,
                                                1,
                                                0,
                                            },
                                            ndmath::vector{0, 1, 0},
                                            ndmath::vector{1, 1, 0}},
                                           {ndmath::vector{
                                                -1,
                                                0,
                                                0,
                                            },
                                            ndmath::vector{0, 0, 0},
                                            ndmath::vector{1, 0, 0}},
                                           {ndmath::vector{
                                                -1,
                                                -1,
                                                0,
                                            },
                                            ndmath::vector{0, -1, 0},
                                            ndmath::vector{1, -1, 0}},
                                       },
                                       {
                                           {ndmath::vector{
                                                -1,
                                                1,
                                                0,
                                            },
                                            ndmath::vector{0, 1, 0},
                                            ndmath::vector{1, 1, 0}},
                                           {ndmath::vector{
                                                -1,
                                                0,
                                                0,
                                            },
                                            ndmath::vector{0, 0, 0},
                                            ndmath::vector{1, 0, 0}},
                                           {ndmath::vector{
                                                -1,
                                                -1,
                                                0,
                                            },
                                            ndmath::vector{0, -1, 0},
                                            ndmath::vector{1, -1, 0}},
                                       }});

    SUBCASE("deltas")
    {
        CHECK_EQ(cartesian_correction.deltas()({0, 0, 0}),
                 ralgo::vector<double>{-1, 1, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 0, 1}),
                 ralgo::vector<double>{0, 1, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 0, 2}),
                 ralgo::vector<double>{1, 1, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 1, 0}),
                 ralgo::vector<double>{-1, 0, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 1, 1}),
                 ralgo::vector<double>{0, 0, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 1, 2}),
                 ralgo::vector<double>{1, 0, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 2, 0}),
                 ralgo::vector<double>{-1, -1, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 2, 1}),
                 ralgo::vector<double>{0, -1, 0});
        CHECK_EQ(cartesian_correction.deltas()({0, 2, 2}),
                 ralgo::vector<double>{1, -1, 0});
    }

    SUBCASE("point_indices")
    {
        auto idxs =
            cartesian_correction.grid().point_in_cell_indices({7, 15, 25});
        CHECK_EQ(idxs, ralgo::vector<size_t>{2, 2, 2});

        idxs =
            cartesian_correction.grid().point_in_cell_indices({-7, -15, -25});
        CHECK_EQ(idxs, ralgo::vector<size_t>{1, 1, 1});
    }

    SUBCASE("cellzone")
    {
        auto idxs =
            cartesian_correction.grid().point_in_cell_indices({-7, 15, -25});
        auto cell = cartesian_correction.grid().cellzone(idxs);

        CHECK_EQ(cell.maxs_as_vector(), ralgo::vector<double>{0, 20, 0});
        CHECK_EQ(cell.mins_as_vector(), ralgo::vector<double>{-10, 0, -30});

        CHECK_EQ(idxs, ralgo::vector<size_t>{1, 2, 1});
        CHECK_EQ(cell.lerpcoeffs({-5, 10, -15}),
                 ralgo::vector<double>{0.5, 0.5, 0.5});
    }
}

TEST_CASE("correction. grid intersect")
{

    ndmath::cartesian_correction cartesian_correction;
    cartesian_correction.set_grid(
        {{-10, 0, 10}, {-20, 0, 20}, {-30, 0, 30, 50}});

    auto [values, indexes] = cartesian_correction.enumerated_sort({3, 2, 0, 1});
    CHECK_EQ(values, ralgo::vector<double>{0, 1, 2, 3});
    CHECK_EQ(indexes, ralgo::vector<size_t>{2, 3, 1, 0});

    {
        auto apnt = ndmath::point{0, 0, -60};
        auto bpnt = ndmath::point{0, 0, 60};
        auto pnts =
            cartesian_correction.line_and_grid_collisions(apnt, bpnt, true);
        CHECK_EQ(pnts[0], ndmath::point{0, 0, -60});
        CHECK_EQ(pnts[1], ndmath::point{0, 0, -30});
        CHECK_EQ(pnts[2], ndmath::point{0, 0, 0});
        CHECK_EQ(pnts[3], ndmath::point{0, 0, 30});
        CHECK_EQ(pnts[4], ndmath::point{0, 0, 50});
        CHECK_EQ(pnts[5], ndmath::point{0, 0, 60});
    }

    {
        auto apnt = ndmath::point{0, -60, 0};
        auto bpnt = ndmath::point{0, 60, 0};
        auto pnts =
            cartesian_correction.line_and_grid_collisions(apnt, bpnt, true);
        CHECK_EQ(pnts[0], ndmath::point{0, -60, 0});
        CHECK_EQ(pnts[1], ndmath::point{0, -20, 0});
        CHECK_EQ(pnts[2], ndmath::point{0, 0, 0});
        CHECK_EQ(pnts[3], ndmath::point{0, 20, 0});
        CHECK_EQ(pnts[4], ndmath::point{0, 60, 0});
    }

    {
        auto apnt = ndmath::point{0, 0, 0};
        auto bpnt = ndmath::point{0, 60, 0};
        auto pnts =
            cartesian_correction.line_and_grid_collisions(apnt, bpnt, true);
        CHECK_EQ(pnts[0], ndmath::point{0, 0, 0});
        CHECK_EQ(pnts[1], ndmath::point{0, 20, 0});
        CHECK_EQ(pnts[2], ndmath::point{0, 60, 0});
    }

    {
        auto apnt = ndmath::point{-10, -20, 0};
        auto bpnt = ndmath::point{20, 40, 0};
        auto pnts =
            cartesian_correction.line_and_grid_collisions(apnt, bpnt, true);
        nos::println(pnts);
        CHECK_EQ(pnts[0], ndmath::point{-10, -20, 0});
        CHECK_EQ(pnts[1], ndmath::point{0, 0, 0});
        CHECK_EQ(pnts[2], ndmath::point{10, 20, 0});
        CHECK_EQ(pnts[3], ndmath::point{20, 40, 0});
    }
}