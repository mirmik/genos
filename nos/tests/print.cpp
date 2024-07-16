#include <doctest/doctest.h>
#include <nos/fprint.h>
#include <nos/io/sstream.h>
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
};
struct C
{
    int i = 42;
};
struct D
{
    int i = 42;
};

int nos_print(nos::ostream &os, const B &b)
{
    return *nos::print_to(os, b.i);
}
std::ostream &operator<<(std::ostream &os, const C &c)
{
    return os << c.i;
}
namespace nos
{
    template <> struct print_implementation<D>
    {
        static int print_to(nos::ostream &os, const D &d)
        {
            return *nos::print_to(os, d.i);
        }
    };
}

TEST_CASE("fprint")
{
    nos::stringstream writer;
    nos::current_ostream = &writer;
    std::string &output = writer.str();

    SUBCASE("print")
    {
        nos::print("print");
        CHECK_EQ(writer.str(), "print");
    }

    SUBCASE("integer")
    {
        nos::print(88);
        CHECK_EQ(writer.str(), "88");
    }

    SUBCASE("number")
    {
        nos::print(0.88);
        CHECK_EQ(writer.str()[0], '0');
        CHECK_EQ(writer.str()[1], '.');
        CHECK_EQ(writer.str()[2], '8');
        CHECK_EQ(writer.str()[3], '8');
    }

    SUBCASE("method")
    {
        nos::print(A());
        CHECK_EQ(output, "42");
    }

    SUBCASE("nos_print_test")
    {
        nos::print(B());
        CHECK_EQ(output, "42");
    }

    SUBCASE("std_ostream")
    {
        nos::print(C());
        CHECK_EQ(output, "42");
    }

    SUBCASE("std_pair")
    {
        nos::print(std::make_pair(33, 55));
        CHECK_EQ(output, "{33,55}");
    }

    SUBCASE("print.numeric")
    {
        nos::print(std::numeric_limits<double>::max());
        nos::print(std::numeric_limits<double>::min());
        nos::print(std::numeric_limits<double>::lowest());

        nos::print(std::numeric_limits<float>::max());
        nos::print(std::numeric_limits<float>::min());
        nos::print(std::numeric_limits<float>::lowest());
    }

    SUBCASE("printhex")
    {
        nos::printhex((char)16);
        CHECK_EQ(output, "10");
    }

    SUBCASE("print_int64_t")
    {
        nos::print((int64_t)16);
        CHECK_EQ(output, "16");
    }

    SUBCASE("print_inf")
    {
        nos::print(2.0 / 0.0);
        CHECK_EQ(output, "+inf");
    }

    SUBCASE("print_-inf")
    {
        nos::print(-2.0 / 0.0);
        CHECK_EQ(output, "-inf");
    }
    SUBCASE("fprint_inf")
    {
        nos::fprint("{}", 2.0 / 0.0);
        CHECK_EQ(output.size(), 4);
        CHECK_EQ(output, "+inf");
    }

    SUBCASE("fprint_-inf")
    {
        nos::fprint("{}", -2.0 / 0.0);
        CHECK_EQ(output, "-inf");
    }

    SUBCASE("print_nan")
    {
        nos::print((2.0 / 0.0) / (2.0 / 0.0));
        CHECK_EQ(output, "nan");
    }
}
