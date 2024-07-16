#include <doctest/doctest.h>
#include <nos/serialize/pbjson.h>
#include <nos/trent/pbjson.h>

class PbjsonTestObject
{
public:
    int a = 0;
    int b = 0;
    std::string c = "initial";

public:
    PbjsonTestObject() : a(0), b(0), c("initial"){};
    PbjsonTestObject(int a, float b, const std::string &c)
        : a(a), b((int)b), c(c)
    {
    }

    template <class Reflect> auto reflect(Reflect &r)
    {
        return r.reflect("a", a) && r.reflect("b", b) && r.reflect("c", c);
    }
};

TEST_CASE("pbjson_test_get_type")
{
    std::string data;
    data.resize(2);
    data[0] = 0x21;
    data[1] = 0x0A;
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    CHECK_EQ(parser.buf[0], 0x21);
    CHECK_EQ(parser.buf[1], 0x0A);
    auto [type, len] = parser.get_type_and_length();
    CHECK_EQ(type, 2);
    CHECK_EQ(len, 1);
}

TEST_CASE("pbjson_test_int")
{
    std::string data("\x21\x0A");
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    nos::trent tr = parser.parse();
    CHECK_EQ(tr.as_numer(), 10);
}

/*TEST_CASE("pbjson_test_double")
{
    std::string data("\x62\x4d\x5d");
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    nos::trent tr = parser.parse();
    CHECK_EQ(tr.as_numer(), 4.5);
}*/

TEST_CASE("pbjson_test_int_two_byte")
{
    std::string data("\x22\x01\x0A");
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    nos::trent tr = parser.parse();
    CHECK_EQ(tr.as_numer(), 266);
}

TEST_CASE("pbjson_test_string")
{
    std::string data("\x85hello");
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    nos::trent tr = parser.parse();
    CHECK_EQ(tr.as_string(), "hello");
}

TEST_CASE("pbjson_test_dict")
{
    std::string data("\xE1\x05hello\x85world");
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    nos::trent tr = parser.parse();
    CHECK_EQ(tr["hello"].as_string(), "world");
}

TEST_CASE("pbjson_test_array")
{
    std::string data("\xC2\x85hello\x85world");
    nos::pbjson::parser parser({(char *)data.data(), data.size()});
    nos::trent tr = parser.parse();
    CHECK_EQ(tr[0].as_string(), "hello");
    CHECK_EQ(tr[1].as_string(), "world");
}

TEST_CASE("pbjson_test_object")
{
    std::string data("\x22\x01\x0A\x41\x0A\x85hello");
    nos::pbjson_deserializer deserializer({(char *)data.data(), data.size()});
    PbjsonTestObject a;
    bool success = a.reflect(deserializer);
    CHECK_EQ(success, true);
    CHECK_EQ(a.a, 266);
    CHECK_EQ(a.b, -10);
    CHECK_EQ(a.c, "hello");
}