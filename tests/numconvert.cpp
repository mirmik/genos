#include <doctest/doctest.h>
#include <igris/util/numconvert.h>
#include <math.h>
#include <string.h>

TEST_CASE("numconvert")
{
    char buf[256];
    float f;
    int i;
    double d;

    i = 42;
    igris_i32toa(i, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    i = -42;
    igris_i32toa(i, buf, 10);
    CHECK(strcmp(buf, "-42") == 0);

    f = 0.1f;
    igris_f32toa(f, buf, 1);
    CHECK(strcmp(buf, "0.1") == 0);

    f = 0.1f;
    igris_f32toa(f, buf, 10);
    CHECK(strcmp(buf, "0.1000000000") == 0);

    f = nanf("");
    igris_f32toa(f, buf, 10);
    CHECK(strcmp(buf, "nan") == 0);

    d = nanf("");
    igris_f64toa(d, buf, 10);
    CHECK(strcmp(buf, "nan") == 0);

    CHECK_EQ(10042, igris_atou32("10042", 10, nullptr));
    CHECK_EQ(10042, igris_atoi32("10042", 10, nullptr));

    CHECK_EQ(igris_atof64("307582293.333333", NULL), 307582293.333333);

    CHECK_EQ(igris_atof64("56789", NULL), 56789);
    CHECK_EQ(igris_atof64("-56789", NULL), -56789);

    CHECK_EQ(igris_atoi32("1", 10, NULL), 1);
    CHECK_EQ(igris_atoi64("1", 10, NULL), 1);
    CHECK_EQ(igris_atou32("1", 10, NULL), 1);
    CHECK_EQ(igris_atou64("1", 10, NULL), 1);
    CHECK_EQ(igris_atof32("1", NULL), 1);
    CHECK_EQ(igris_atof64("1", NULL), 1);
    CHECK_EQ(igris_atoi32("-1", 10, NULL), -1);
    CHECK_EQ(igris_atoi64("-1", 10, NULL), -1);
    CHECK_EQ(igris_atof32("-1", NULL), -1);
    CHECK_EQ(igris_atof64("-1", NULL), -1);
    CHECK_EQ(igris_atoi32("1.", 10, NULL), 1);
    // CHECK_EQ(igris_atoi64("1.0", 10, NULL), 1);
    // CHECK_EQ(igris_atou32("1.0", 10, NULL), 1);
    // CHECK_EQ(igris_atou64("1.0", 10, NULL), 1);
    // CHECK_EQ(igris_atof32("1.0", NULL), 1);
    // CHECK_EQ(igris_atof64("1.0", NULL), 1);
}

TEST_CASE("numconvert check all functions")
{
    char buf[256];

    CHECK_EQ(igris_atoi8("42", 10, nullptr), 42);
    CHECK_EQ(igris_atoi16("42", 10, nullptr), 42);
    CHECK_EQ(igris_atoi32("42", 10, nullptr), 42);
    CHECK_EQ(igris_atoi64("42", 10, nullptr), 42);

    CHECK_EQ(igris_atou8("42", 10, nullptr), 42);
    CHECK_EQ(igris_atou16("42", 10, nullptr), 42);
    CHECK_EQ(igris_atou32("42", 10, nullptr), 42);
    CHECK_EQ(igris_atou64("42", 10, nullptr), 42);

    CHECK_EQ(igris_atof32("42", nullptr), 42);
    CHECK_EQ(igris_atof64("42", nullptr), 42);

    igris_i8toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_i16toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_i32toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_i64toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_u8toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_u16toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_u32toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_u64toa(42, buf, 10);
    CHECK(strcmp(buf, "42") == 0);

    igris_f32toa(42, buf, 10);
    CHECK(strcmp(buf, "42.0000000000") == 0);

    igris_f64toa(42, buf, 10);
    CHECK(strcmp(buf, "42.0000000000") == 0);
}
