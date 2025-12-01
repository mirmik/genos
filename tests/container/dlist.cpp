#include <doctest/doctest.h>
#include <igris/container/dlist.h>

class dlist_test_node
{
public:
    igris::dlist_node node = {};
    dlist_test_node() = default;
};

TEST_CASE("dlist_node")
{
    igris::dlist_node node;
    CHECK(node.next_node() == &node);
    CHECK(node.prev_node() == &node);
}

TEST_CASE("dlist")
{
    dlist_test_node node1;
    dlist_test_node node2;
    dlist_test_node node3;

    igris::dlist<dlist_test_node, &dlist_test_node::node> list;

    list.move_back(node1);
    list.move_back(node2);
    list.move_back(node3);

    CHECK(list.front().node.next_node() == &node2.node);
    CHECK_EQ(list.size(), 3);

    list.pop_front();
    CHECK_EQ(list.size(), 2);
}

TEST_CASE("dlist.empty")
{
    dlist_test_node node1;
    dlist_test_node node2;
    dlist_test_node node3;

    igris::dlist<dlist_test_node, &dlist_test_node::node> list;
    CHECK(list.empty());

    list.move_back(node1);
    list.move_back(node2);
    list.move_back(node3);

    CHECK(!list.empty());

    list.pop_front();
    CHECK(!list.empty());

    list.pop_front();
    CHECK(!list.empty());

    list.pop_front();
    CHECK(list.empty());
}