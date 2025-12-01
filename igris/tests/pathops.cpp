#include <doctest/doctest.h>
#include <igris/util/pathops.h>

#include <stdio.h>

TEST_CASE("pathops.iterate")
{
    const char *b = "/dev/null";

    b = path_iterate(b);
    CHECK(strcmp(b, "dev/null") == 0);

    b = path_iterate(b);
    CHECK(strcmp(b, "null") == 0);

    b = path_iterate(b);
    CHECK(strcmp(b, "") == 0);

    b = path_iterate(b);
    CHECK_EQ(b, (const char *)NULL);

    b = path_iterate(b);
    CHECK_EQ(b, (const char *)NULL);
}

TEST_CASE("pathops.compare_node")
{
    const char *a = "dev/null";
    const char *b = "dev";

    CHECK(path_compare_node(a, b) == 0);
}

TEST_CASE("pathops.remove_prefix")
{
    const char *a = "/dev/null";
    const char *b = "/dev";
    const char *c;

    c = path_remove_prefix(a, b);

    CHECK(strcmp(c, "null") == 0);
}

TEST_CASE("pathops.remove_prefix_2")
{
    const char *a = "/dev/hello/mirm/null";
    const char *b = "/dev/hello/";
    const char *c;

    c = path_remove_prefix(a, b);

    CHECK(strcmp(c, "mirm/null") == 0);
}

TEST_CASE("pathops.remove_prefix_3")
{
    const char *a = "/dev/hello/mirm/null";
    const char *b = "/dev/hello/lalal";
    const char *c;

    c = path_remove_prefix(a, b);

    CHECK(strcmp(c, "mirm/null") == 0);
}
