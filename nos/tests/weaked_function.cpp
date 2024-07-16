#include <cmath>
#include <doctest/doctest.h>
#include <nos/shell/weaked_function.h>
#include <nos/shell/weaked_function_interpreter.h>

class TestComfortableFunction
{
public:
    int a = 0;
    int b = 0;

    TestComfortableFunction() = default;
    TestComfortableFunction(int a, int b) : a(a), b(b) {}

public:
    int sum(int c)
    {
        return a + b + c;
    }
};

int sum(int a, int b)
{
    return a + b;
}

TEST_CASE("kwargs")
{
    nos::weaked_function<int(int, int)> func(
        [](int a, int b) { return (int)std::pow(a, b); }, {"a", "b"});

    nos::weaked_function<int(nos::trent)> bar(
        [](nos::trent a) { return (int)(a[0].as_numer() * a[1].as_numer()); },
        {"a"});

    CHECK_EQ(func(2, 3).value(), 8);
    CHECK_EQ(func(nos::argpair("a", 2), 3).value(), 8);
    CHECK_EQ(func(2, nos::argpair("b", 3)).value(), 8);
    CHECK_EQ(func(nos::argpair("a", 2), nos::argpair("b", 3)).value(), 8);
    CHECK_EQ(func(nos::argpair("b", 3), nos::argpair("a", 2)).value(), 8);

    CHECK_EQ(func("2", 3).value(), 8);
    CHECK_EQ(func(nos::argpair("a", "2"), 3).value(), 8);
    CHECK_EQ(func("2", nos::argpair("b", 3)).value(), 8);
    CHECK_EQ(func(nos::argpair("b", "3"), nos::argpair("a", 2)).value(), 8);

    nos::trent t;
    t.push_back(2);
    t.push_back("3");
    CHECK_EQ(bar(t).value(), 6);

    TestComfortableFunction tcf(1, 2);
    nos::weaked_function<int(int)> test_func(
        [&tcf](int c) { return tcf.sum(c); }, {"c"});

    CHECK_EQ(test_func(3).value(), 6);
}

TEST_CASE("make_cf_abstract")
{
    nos::wf_collection collection;
    collection.add("sum", std::function<int(int, int)>(sum));
    collection.add("sub", std::function<int(int, int)>([](int a, int b) {
                       return a - b;
                   }));
    collection.add("pow",
                   std::function<int(int, int)>(
                       [](int a, int b) { return (int)std::pow(a, b); }),
                   {"a", "b"});
    collection.add(
        "stub", std::function<void(int, int)>([](int, int) {}), {"a", "b"});

    CHECK_EQ(
        collection
            .execute("sum", {nos::trent_argument{3}, nos::trent_argument{7}})
            .value()
            .as_numer(),
        10);
    CHECK_EQ(
        collection
            .execute("sub", {nos::trent_argument{3}, nos::trent_argument{7}})
            .value()
            .as_numer(),
        -4);
    CHECK_EQ(
        collection
            .execute("pow",
                     {nos::trent_argument{7, "b"}, nos::trent_argument{3, "a"}})
            .value()
            .as_numer(),
        2187);

    CHECK_EQ(
        collection
            .execute("sub", {nos::trent_argument{"3"}, nos::trent_argument{7}})
            .value()
            .as_numer(),
        -4);

    TestComfortableFunction tcf(2, 3);
    collection.add(
        "tcf", std::function<int(int)>([&tcf](int c) { return tcf.sum(c); }));
    CHECK_EQ(
        collection.execute("tcf", {nos::trent_argument{4}}).value().as_numer(),
        9);
}

TEST_CASE("nos::weak_function_interpreter console command protocol")
{
    nos::weaked_function_interpreter interpreter;
    interpreter.add("sum", std::function<int(int, int)>(sum), {"a", "b"});
    interpreter.add("pow",
                    std::function<int(int, int)>(
                        [](int a, int b) { return (int)std::pow(a, b); }),
                    {"a", "b"});

    {
        auto result = *interpreter.execute_console_command_protocol("sum 3 7");
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 10);
    }

    {
        auto result =
            *interpreter.execute_console_command_protocol("sum --a 3 --b 7");
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 10);
    }

    {
        auto result =
            *interpreter.execute_console_command_protocol("pow --a 2 --b 3");
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 8);
    }

    {
        auto result =
            *interpreter.execute_console_command_protocol("pow --a 3 --b 2");
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 9);
    }
}

TEST_CASE("nos::weak_function_interpreter json protocol")
{
    nos::weaked_function_interpreter interpreter;
    interpreter.add("sum", std::function<int(int, int)>(sum));
    interpreter.add("pow",
                    std::function<int(int, int)>(
                        [](int a, int b) { return (int)std::pow(a, b); }),
                    {"a", "b"});

    {
        auto tr = nos::json::parse(R"({"cmd": "sum", "args": [3, 7]})");
        auto result = *interpreter.execute_json_protocol(tr);
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 10);
    }

    {
        auto tr =
            nos::json::parse(R"({"cmd": "pow", "kwargs": {"a":2, "b":3}})");
        auto result = *interpreter.execute_json_protocol(tr);
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 8);
    }

    {
        auto tr =
            nos::json::parse(R"({"cmd": "pow", "kwargs": {"a":3, "b":2}})");
        auto result = *interpreter.execute_json_protocol(tr);
        auto result_num = result.as_numer();
        CHECK_EQ(result_num, 9);
    }
}