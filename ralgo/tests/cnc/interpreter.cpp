#include <doctest/doctest.h>
#include <ralgo/cnc/interpreter.h>

TEST_CASE("interpreter")
{
    igris::ring<cnc::planner_block> blocks_ring(10);
    cnc::interpreter interpreter(&blocks_ring, nullptr, nullptr, nullptr);
}
