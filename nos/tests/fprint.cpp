#include <charconv>
#include <doctest/doctest.h>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
#include <nos/io/string_writer.h>
#include <nos/print.h>

struct A
{
    int i = 42;
    int print_to(nos::ostream &os) const
    {
        return *nos::print_to(os, i);
    };
};
struct B
{
    int i = 42;

    template <class Out> int print_to(Out &os) const
    {
        return *nos::print_to(os, i);
    }
};

struct C
{
    int i = 42;

    template <class Out>
    nos::expected<size_t, nos::output_error> print_to(Out &os) const
    {
        return *nos::print_to(os, i);
    }
};

using namespace nos::argument_literal;

TEST_CASE("fprint")
{
    nos::stringstream writer;
    std::string &output = writer.str();
    nos::current_ostream = &writer;

    SUBCASE("fprint(A)")
    {
        nos::fprint("{}", A());
        CHECK_EQ(output, "42");
    }

    SUBCASE("fprint(B)")
    {
        nos::fprint("{}", B());
        CHECK_EQ(output, "42");
    }

    SUBCASE("fprint(C)")
    {
        nos::fprint("{}", C());
        CHECK_EQ(output, "42");
    }

    SUBCASE("fprint")
    {
        nos::fprint("fprint {} {} {}", "aaa", "bbb", "ccc");
        CHECK_EQ(output, "fprint aaa bbb ccc");
    }

    SUBCASE("fprint_integer")
    {
        nos::fprint("integer({})", 42);
        CHECK_EQ(output, "integer(42)");
    }

    SUBCASE("fprint_named")
    {
        nos::fprint(
            "fprint {a} {b} {c}", "a"_a = "aaa", "b"_a = "bbb", "c"_a = "ccc");
        CHECK_EQ(output, "fprint aaa bbb ccc");
    }

    SUBCASE("fprint_named_and_numers1")
    {
        nos::fprint(
            "fprint {b} {2} {0}", "a"_a = "aaa", "b"_a = "bbb", "c"_a = "ccc");
        CHECK_EQ(output, "fprint bbb ccc aaa");
    }

    SUBCASE("fprint_named_and_numers3")
    {
        nos::fprint("fprint {a1}", "a1"_a = "aaa");
        CHECK_EQ(output, "fprint aaa");
    }

    SUBCASE("fprint_zero_size_arglist")
    {
        nos::fprint("zerosize");
        CHECK_EQ(output, "zerosize");
    }

    SUBCASE("fprint_named_and_numers2")
    {
        nos::fprint("fprint {a1} {b2}", "a1"_a = "aaa", "b2"_a = "bbb");
        CHECK_EQ(output, "fprint aaa bbb");
    }

    SUBCASE("format_named_and_numers2")
    {
        auto str =
            nos::format("fprint {a1} {b2}", "a1"_a = "aaa", "b2"_a = "bbb");
        CHECK_EQ(str, "fprint aaa bbb");
    }

    SUBCASE("fprint_longstring")
    {
        nos::fprint("1234567890123456789012345678901234567890123456789012345678"
                    "90123456789012345678901234567890");
        CHECK_EQ(output,
                 "1234567890123456789012345678901234567890123456789012345678901"
                 "23456789012345678901234567890");
    }

    SUBCASE("format")
    {
        std::string out = nos::format("format {} {} {}", "aaa", "bbb", "ccc");
        CHECK_EQ(out, "format aaa bbb ccc");
    }

    SUBCASE("format.zero")
    {
        std::string out = nos::format("format {:05>}", "aaa");
        CHECK_EQ(out, "format 00aaa");
    }

    SUBCASE("format cmd")
    {
        auto cmd = nos::format("cnc G01 {poses} {speed} {accel}",
                               "poses"_a = "Hello",
                               "speed"_a = "World",
                               "accel"_a = "!");
        CHECK_EQ(cmd, "cnc G01 Hello World !");
    }

    SUBCASE("format_nan")
    {
        std::string out = nos::format("{}", std::nan("fsdfasf"));
        CHECK_EQ(out, "nan");
    }

    SUBCASE("format_nanf")
    {
        std::string out = nos::format("{}", std::nanf("afdsfasd"));
        CHECK_EQ(out, "nan");
    }

    SUBCASE("format_string_arg")
    {
        std::string out = nos::format("{}", std::string("hello"));
        CHECK_EQ(out, "hello");
    }

    SUBCASE("format_int64")
    {
        std::string out = nos::format("{}", int64_t(42));
        CHECK_EQ(out, "42");
    }

#if __has_include(<string_view>)
    SUBCASE("format_string_view_arg")
    {
        std::string str = "hello";
        std::string out = nos::format("{}", std::string_view(str));
        CHECK_EQ(out, "hello");
    }
#endif

    SUBCASE("fprint.numeric")
    {
        nos::fprint("{}", std::numeric_limits<double>::max());
        nos::fprint("{}", std::numeric_limits<double>::min());
        nos::fprint("{}", std::numeric_limits<double>::lowest());

        nos::fprint("{}", std::numeric_limits<float>::max());
        nos::fprint("{}", std::numeric_limits<float>::min());
        nos::fprint("{}", std::numeric_limits<float>::lowest());

        nos::fprint("{:.3}", std::numeric_limits<double>::max());
        nos::fprint("{:.3}", std::numeric_limits<double>::min());
        nos::fprint("{:.3}", std::numeric_limits<double>::lowest());

        nos::fprint("{:.3}", std::numeric_limits<float>::max());
        nos::fprint("{:.3}", std::numeric_limits<float>::min());
        nos::fprint("{:.3}", std::numeric_limits<float>::lowest());
    }
}

TEST_CASE("__nos_dtoa")
{
    char buf[100];
    double obj = 123.456789;
    __nos_dtoa(obj, buf, 6);
    CHECK_EQ(std::string(buf), "123.456789");
}

TEST_CASE("format hex")
{
    auto ret = nos::format("{:X}", 42);
    CHECK_EQ(ret, "2A");
}

TEST_CASE("format hex")
{
    auto ret = nos::format("{:x}", 42);
    CHECK_EQ(ret, "2a");
}

TEST_CASE("big double")
{
    {
        char buf[128];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%0.5f", (double)213241278.499228);
        CHECK_EQ(std::string(buf), "213241278.49923");
    }
    {
        std::string out = nos::format("{}", (double)213241278.499228);
        CHECK_EQ(out, "213241278.499228");
    }
    {
        std::string out = nos::format("{:.5}", 10213241278.499228);
        CHECK_EQ(out, "10213241278.49923");
    }
}

TEST_CASE("format string with string")
{
    auto s = nos::format("{} {}", std::string("Hello"), std::string("World"));
    CHECK_EQ(s, "Hello World");
}