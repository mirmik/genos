#include <igris/event/safe_queue.h>
#include <doctest/doctest.h>

TEST_CASE("safe_queue") 
{
    igris::safe_queue<int> queue;

    queue.push(1);
    queue.push(2);
    queue.push(3);

    CHECK(queue.pop() == 1);
    CHECK(queue.pop() == 2);
    CHECK(queue.pop() == 3);
}