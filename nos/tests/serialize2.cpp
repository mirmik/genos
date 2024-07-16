#include <doctest/doctest.h>
#include <nos/reflect.h>
#include <nos/trent/trent.h>
#include <nos/trent/trent_serializer.h>

class Ab
{
public:
    int a = 0;
    float b = 0;
    std::string c = "initial";

public:
    Ab() : a(0), b(0), c("initial"){};
    Ab(int a, float b, const std::string &c) : a(a), b(b), c(c) {}

    template <class Reflect> auto reflect(Reflect &r)
    {
        return r.reflect("a", a) && r.reflect("b", b) && r.reflect("c", c);
    }
};

TEST_CASE("serialize_test")
{
    CHECK_EQ(nos::has_reflect_method<Ab>::value, true);
}

TEST_CASE("serialize_trent")
{
    nos::trent tr;
    nos::trent_object_serializer serializer(tr);
    Ab a(3, 4, "hello");
    serializer.reflect(a);
    CHECK_EQ(tr["a"].as_numer(), 3);
    CHECK_EQ(tr["b"].as_numer(), 4);
    CHECK_EQ(tr["c"].as_string(), "hello");
}

TEST_CASE("deserialize_trent_string")
{
    nos::trent tr;
    tr = "hello";
    nos::trent_object_deserializer deserializer(tr);
    std::string a;
    deserializer.reflect(a);
    CHECK_EQ(a, "hello");
}

TEST_CASE("deserialize_trent")
{
    Ab a;
    CHECK_EQ(a.a, 0);
    CHECK_EQ(a.b, 0);
    CHECK_EQ(a.c, "initial");
    nos::trent tr;
    tr["a"] = 3;
    tr["b"] = 4;
    tr["c"] = "hello";
    nos::trent_object_deserializer deserializer(tr);
    deserializer.reflect(a);
    CHECK_EQ(a.a, 3);
    CHECK_EQ(a.b, 4);
    CHECK_EQ(a.c, "hello");
}