#include <doctest/doctest.h>
#include <igris/datastruct/nametbl.h>

#include <stdint.h>

TEST_CASE("nametbl")
{
    struct nametbl_node table[] = {
        {"1", (void *)1}, {"2", (void *)2}, {NULL, NULL}};

    struct nametbl2_node table2[] = {
        {"1", (void *)1, NULL}, {"2", (void *)2, NULL}, {NULL, NULL}};

    CHECK_NE(nametbl_find(table, "1"), nullptr);
    CHECK_NE(nametbl2_find(table2, "2"), nullptr);

    CHECK_EQ((uintptr_t)nametbl_find(table, "1")->data, 1);
    CHECK_EQ((uintptr_t)nametbl2_find(table2, "2")->data0, 2);
}
