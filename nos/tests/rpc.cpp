#include <doctest/doctest.h>
#include <nos/shell/rpc/rpc.h>
#include <nos/shell/rpc/rpc_executor.h>

static int sum(int a, int b)
{
    return a + b;
}

TEST_CASE("bytearray")
{
    nos::bytearray array;
    nos::bytearray_ostream os(array);
    nos::print_to(os, "lalala");
    CHECK_EQ(array.size(), 6);
    nos::bytearray_istream is(array);
    char buf[6];
    is.read(buf, 6);
    CHECK_EQ(buf[0], 'l');
    CHECK_EQ(buf[1], 'a');
    CHECK_EQ(buf[2], 'l');
    CHECK_EQ(buf[3], 'a');
    CHECK_EQ(buf[4], 'l');
    CHECK_EQ(buf[5], 'a');
}

TEST_CASE("rpc")
{
    nos::rpc_executor<std::string> executor;
    executor.add(std::string("sum"), std::function<int(int, int)>(sum));

    SUBCASE("M")
    {
        CHECK_EQ(executor.methods().size(), 1);
        CHECK_NE(executor.find("sum"), nullptr);
    }

    SUBCASE("S")
    {
        auto bind =
            nos::serialize_remote_call<std::string, int, int>("bug", 1, 2);
        nos::bytearray_istream reader(bind);
        CHECK_EQ(bind.size(), 1 + 1 + 3 + 4 + 4);
        CHECK_EQ(bind[0], 1);
        CHECK_EQ(bind[1], 3);
        CHECK_EQ(bind[2], 'b');
        CHECK_EQ(bind[3], 'u');
        CHECK_EQ(bind[4], 'g');
        auto id = nos::deserialize_from<std::string>(reader);
        CHECK_EQ(id, "bug");
    }

    SUBCASE("A")
    {
        auto bind =
            nos::serialize_remote_call<std::string, int, int>("bug", 1, 2);
        auto ret = executor.execute(bind);
        CHECK_EQ(ret.size(), 2);
        auto errcode = nos::deserialize<int16_t>(ret);
        CHECK_EQ(errcode, (int16_t)nos::RpcError::MethodNotFoundCode);
    }

    SUBCASE("B")
    {
        auto bind =
            nos::serialize_remote_call<std::string, int, int>("sum", 1, 2);
        auto ret = executor.execute(bind);
        CHECK_EQ(ret.size(), 6);
        nos::bytearray_istream reader(ret);
        auto errcode = nos::deserialize_from<int16_t>(reader);
        CHECK_EQ(errcode, (int16_t)nos::RpcError::OK);
        auto object = nos::deserialize_from<int>(reader);
        CHECK_EQ(errcode, 0);
        CHECK_EQ(object, 3);
    }
}