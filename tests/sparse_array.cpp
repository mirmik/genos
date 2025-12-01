#include <doctest/doctest.h>
#include <igris/datastruct/sparse_array.h>

struct AA
{
    int i;
    int j;
};

TEST_CASE("sparse_array")
{
    AA a[2];

    a[0].i = 1;
    a[0].j = 2;
    a[1].i = 3;
    a[1].j = 4;

    SPARSE_ARRAY(array0, &a[0].i, sizeof(AA));
    SPARSE_ARRAY(array1, &a[0].j, sizeof(AA));

    CHECK_EQ(sparse_array_ref(&array0, 0, int), 1);
    CHECK_EQ(sparse_array_ref(&array0, 1, int), 3);
    CHECK_EQ(sparse_array_ref(&array1, 0, int), 2);
    CHECK_EQ(sparse_array_ref(&array1, 1, int), 4);

    int *it = &a[0].i;
    CHECK_EQ(sparse_array_next(it, array0.stride), &a[1].i);

    int acc = 0;
    sparse_array_for_each(it, &array0, 2)
    {
        acc += *it;
    }
    CHECK_EQ(acc, 1 + 3);

    acc = 0;
    sparse_array_for_each(it, &array1, 2)
    {
        acc += *it;
    }
    CHECK_EQ(acc, 2 + 4);
}
