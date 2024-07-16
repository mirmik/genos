#include <nos/serialize/serialize.h>
#include <nos/serialize/deserialize.h>
#include <doctest/doctest.h>
#include <nos/io/sstream.h>

class TestSerializeObject 
{
public:
    int a = 10;
    double b = 10;
    int c = 10;

public:
    TestSerializeObject() = default;
    TestSerializeObject(int a, double b, int c) : a(a), b(b), c(c) {}

    template <class ReflectionOperator>
    void reflect(ReflectionOperator& reflector)
    {
        reflector(a);
        reflector(b);
        reflector(c);
    }
};

TEST_CASE("serialize") 
{
    nos::stringstream sstr;

    SUBCASE("A") 
    {
        nos::serialize_to<int>(sstr, 12);
        CHECK_EQ(sstr.str().size(), 4);
        CHECK_EQ(sstr.str()[0], 12);
        CHECK_EQ(sstr.str()[1], 0);
        CHECK_EQ(sstr.str()[2], 0);
        CHECK_EQ(sstr.str()[3], 0);
    }

    SUBCASE("A") 
    {
        nos::serialize_to<int>(sstr, 12);
        auto out = nos::deserialize_from<int>(sstr);
        CHECK_EQ(out, 12);
    }

    SUBCASE("B") 
    {
        nos::serialize_to<std::string>(sstr, "lalala");
        auto out = nos::deserialize_from<std::string>(sstr);
        CHECK_EQ(out, "lalala");
    }

    SUBCASE("C") 
    {
        TestSerializeObject obj(2,3,4);
        nos::serialize_to<TestSerializeObject>(sstr, obj);
        TestSerializeObject restored = nos::deserialize_from<TestSerializeObject>(sstr);
        CHECK_EQ(obj.a, restored.a);
        CHECK_EQ(obj.b, restored.b);
        CHECK_EQ(obj.c, restored.c);
    }
}


TEST_CASE("rpc") 
{
    
}