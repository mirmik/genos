#include <doctest/doctest.h>
#include <nos/trent/json.h>
#include <sstream>

#define private public

#include <ndmath/named_cartesian_correction.h>

TEST_CASE("named_cartesian_correction")
{
    auto tr = nos::json::parse(R"({
		"names" : [ "X", "Y", "Z"],

	"grid" : [
		[0, 2, 3],
		[0, 1],
        [0]
	],

	"deltas" : [
		[[[0,0,0.10]], [[0,0,-0.22]]], 
		[[[0,0,-0.20]], [[0,0,-0.32]]],
		[[[0,0,0.28]], [[0,0,0.16]]]
	]
}
)");
    auto ret = ndmath::named_cartesian_correction(tr);

    CHECK_EQ(ret.grid[0], std::vector<double>{0, 2, 3});
    CHECK_EQ(ret.grid[1], std::vector<double>{0, 1});
    CHECK_EQ(ret.grid[2], std::vector<double>{0});

    CHECK_EQ(ret.deltas.shape(), std::vector<size_t>{3, 2, 1});
    CHECK_EQ(ret.deltas({0, 0, 0}), ndmath::vector{0, 0, 0.1});
    CHECK_EQ(ret.deltas({1, 1, 0}), ndmath::vector{0, 0, -0.32});
    CHECK_EQ(ret.deltas({1, 0, 0}), ndmath::vector{0, 0, -0.20});
    CHECK_EQ(ret.deltas({2, 1, 0}), ndmath::vector{0, 0, 0.16});

    auto correction = ret.make_correction_table();
    {
        CHECK_EQ(correction.corrected_point(ndmath::point{-0.1, 0, 0}),
                 ndmath::vector{-0.1, 0, 0.1});
    }
}

TEST_CASE("named_cartesian_correction zeros")
{
    auto tr = nos::json::parse(R"({
		"names" : [ "X", "Y", "Z"],

	"grid" : [
		[0, 2, 3],
		[0, 1],
        [0]
	],

	"deltas" : [
		[[[ 0.10, 0, 0]], [[-0.22, 0, 0]]], 
		[[[-0.20, 0, 0]], [[-0.32, 0, 0]]],
		[[[ 0.28, 0, 0]], [[ 0.16, 0, 0]]]
	]
}
)");
    auto ret = ndmath::named_cartesian_correction(tr);

    CHECK_EQ(ret.grid[0], std::vector<double>{0, 2, 3});
    CHECK_EQ(ret.grid[1], std::vector<double>{0, 1});

    CHECK_EQ(ret.deltas.shape(), std::vector<size_t>{3, 2, 1});
    CHECK_EQ(ret.deltas({0, 0, 0}), ndmath::vector{0.1, 0, 0});
    CHECK_EQ(ret.deltas({1, 1, 0}), ndmath::vector{-0.32, 0, 0});
    CHECK_EQ(ret.deltas({1, 0, 0}), ndmath::vector{-0.20, 0, 0});
    CHECK_EQ(ret.deltas({2, 1, 0}), ndmath::vector{0.16, 0, 0});

    auto cortable = ret.make_correction_table();
    {
        CHECK_EQ(cortable.corrected_point(ndmath::point{2, 1, 1}),
                 ndmath::vector{2 - 0.32, 1, 1});
    }
    {
        CHECK_EQ(cortable.corrected_point(ndmath::point{2, 1, 15}),
                 ndmath::vector{2 - 0.32, 1, 15});
    }

    {
        CHECK_EQ(cortable.corrected_point(ndmath::point{4, 1, 1}),
                 ndmath::vector{4.16, 1, 1});
    }

    {
        CHECK_EQ(cortable.corrected_point(ndmath::point{5, 1, 1}),
                 ndmath::vector{5.16, 1, 1});
    }

    {
        auto pnt = ndmath::point{-2, 2, 1};
        auto cell = cortable.cell_for_point(pnt);
        CHECK_EQ(
            cell.mins_as_vector(),
            ndmath::vector{-std::numeric_limits<double>::infinity(), 1, 0});
        CHECK_EQ(cell.maxs_as_vector(),
                 ndmath::vector{0,
                                std::numeric_limits<double>::infinity(),
                                std::numeric_limits<double>::infinity()});
    }

    {
        CHECK_EQ(cortable.corrected_point(ndmath::point{-2, 1, 1}),
                 ndmath::vector{-2 - 0.22, 1, 1});
    }
}

TEST_CASE("named_cartesian_correction.oneaxis")
{
    auto tr = nos::json::parse(R"({
		"names" : ["X"],

	"grid" : [
		[0, 2, 3]
	],

	"deltas" : [
		[ 0.10], 
		[-0.20],
		[ 0.28]
	]
})");
    auto ret = ndmath::named_cartesian_correction(tr);
    CHECK_EQ(ret.grid[0], std::vector<double>{0, 2, 3});

    CHECK_EQ(ret.deltas.shape(), std::vector<size_t>{3});
    CHECK_EQ(ret.deltas({0}), ndmath::vector{0.1});
    CHECK_EQ(ret.deltas({1}), ndmath::vector{-0.2});
    CHECK_EQ(ret.deltas({2}), ndmath::vector{0.28});

    auto cortable = ret.make_correction_table();
    // cortable.corrected_point(ndmath::vector{1});
    CHECK_EQ(cortable.corrected_point(ndmath::vector{1}), ndmath::vector{0.95});
    CHECK_EQ(cortable.corrected_point(ndmath::vector{2.5}),
             ndmath::vector{2.54});
}