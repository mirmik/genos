#include "doctest/doctest.h"
#include <igris/container/ring.h>
#include <igris/datastruct/ring.h>

TEST_CASE("ring")
{
    struct ring_head ring;

    ring_init(&ring, 10);
    ring_move_head(&ring, 4);

    CHECK_EQ(ring_avail(&ring), 4);
    CHECK_EQ(ring_room(&ring), 5);
}
