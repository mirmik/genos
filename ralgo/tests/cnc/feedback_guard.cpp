#include <doctest/doctest.h>
#include <ralgo/cnc/feedback_guard.h>

TEST_CASE("feedback_guard")
{
    cnc::feedback_guard guard(2);
}