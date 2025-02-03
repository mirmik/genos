#include <cmath>
#include <doctest/doctest.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <nos/trent/schema.h>

TEST_CASE("json_test.to_string")
{
    nos::trent tr;
    tr = 13;
    nos::json::to_string(tr);
}

TEST_CASE("json_test")
{
    nos::trent tr;

    tr = nos::json::parse("13");
    CHECK_EQ(tr.as_numer(), 13);

    tr = nos::json::parse("0.5");
    CHECK_EQ(tr.as_numer(), 0.5);

    tr = nos::json::parse("-0.5");
    CHECK_EQ(tr.as_numer(), -0.5);

    tr = nos::json::parse("/*13*/ 42");
    CHECK_EQ(tr.as_numer(), 42);

    tr = nos::json::parse("{'a':42, /*aaa*/ 'b' : 13}");
    CHECK(tr.is_dict());

    tr = nos::json::parse("{'a':42, /*aaa*/ 'b' : 13 }");
    CHECK(tr.is_dict());

    tr = nos::json::parse(R"(
		[42, //aaa 
		13]
	)");
    CHECK(tr.is_list());

    tr = nos::json::parse(R"(

		[42,

	/*aaa*/ 

	13]

	)");
    CHECK(tr.is_list());
}

TEST_CASE("trent_basic_test")
{
    nos::trent tr;

    tr["a"]["b"][28] = 42;
    auto a = tr["a"]["b"][28].as_numer();
    CHECK_EQ(a, 42);
}

TEST_CASE("trent_basic_test")
{
    nos::trent tr;

    tr = nos::json::parse(" 20e-1 ");
    CHECK_EQ(tr.as_numer(), 2.0);
}

TEST_CASE("trent_path")
{
    nos::trent tr;
    nos::trent::numer_type a;

    tr["a"]["b"][28] = 42;
    a = tr[nos::trent_path("a/b/28")].as_numer();
    CHECK_EQ(a, 42);

    tr[28]["a"]["b"] = 42;
    a = tr[nos::trent_path("28/a/b")].as_numer();
    CHECK_EQ(a, 42);

    tr[7] = 42;
    a = tr[nos::trent_path("7")].as_numer();
    CHECK_EQ(a, 42);
}

TEST_CASE("trent_bool")
{
    nos::trent tr;

    tr["a"][28] = true;
    bool a = tr[nos::trent_path("a/28")].as_bool();
    CHECK_EQ(a, true);

    tr["a"][29] = false;
    a = tr[nos::trent_path("a/29")].as_bool();
    CHECK_EQ(a, false);

    nos::trent tr2;
    // tr2 = nos::json::parse(" false ");
    // CHECK(tr2.is_bool());
    // CHECK_EQ(tr2.as_bool(), false);

    tr2 = nos::json::parse(" {'a': false} ");
    CHECK(tr2["a"].is_bool());
    CHECK_EQ(tr2["a"].as_bool(), false);

    tr2 = nos::json::parse(" {'a': false } ");
    CHECK(tr2["a"].is_bool());
    CHECK_EQ(tr2["a"].as_bool(), false);
}

TEST_CASE("get_test")
{
    nos::trent tr;

    tr["a"]["b"] = 3;
    CHECK_EQ(tr.get("a/b"), &tr["a"]["b"]);

    tr[7]["a"]["b"] = 3;
    CHECK_EQ(tr.get("7/a/b"), &tr[7]["a"]["b"]);

    tr["a"]["b"] = 3;
    CHECK_EQ(tr.get_as_numer_ex("a/b"), 3);

    tr["A"][7] = 8;
    CHECK_EQ(tr.get_as_numer_ex("A/7"), 8);

    tr[7] = 8;
    CHECK_EQ(tr.get_as_numer_ex("7"), 8);

    tr[7]["A"] = 8;
    CHECK_EQ(tr.get_as_numer_ex("7/A"), 8);

    tr["a"]["b"][28] = 42;
    CHECK_EQ(tr.get_as_numer_ex("a/b/28"), 42);

    tr["a"]["b"][28] = -123.513;
    CHECK(std::fabs(tr.get_as_numer_ex("a/b/28") + 123.513) < 1e-5);

    CHECK_THROWS(tr.get_as_numer_ex("a/c/28"));

    tr = nos::json::parse(R"(
		{
			"name": "Az",
			"type": "simple",
			"motionType": "round",
			"servo": "srv_axis1",
			"unitRatio": 2056,
			"defaultSpeed_rpm": 202,
			"defaultAccel_ms": 10000,
			"maxSpeed_rpm": 1500,
			"minAccel_ms": 2000,
			"backwardLimit": -100,
			"forwardLimit": 100,
			"reverse": 0
		}
	)");
    CHECK(tr["backwardLimit"].is_numer());
    CHECK_EQ(tr["backwardLimit"].as_numer(), -100);

    try
    {
        tr.get_as_numer_ex("a/c/28");
    }
    catch (std::exception &ex)
    {
        std::string what = ex.what();
        CHECK_EQ(what, std::string("trent:wrong_path: a/c/28"));
    }

    tr["a"]["b"][28] = "hello";
    try
    {
        tr.get_as_numer_ex("a/b/28");
    }
    catch (std::exception &ex)
    {
        std::string what = ex.what();
        CHECK_EQ(what,
                 std::string(
                     "trent:wrong_type: path:a/b/28 request:num realtype:str"));
    }
}

TEST_CASE("trent_order_saving_check")
{
    nos::trent tr;
    std::string json;

    tr["a"] = 1;
    tr["c"] = 3;
    tr["b"] = 2;

    json = nos::json::to_string(tr);
    CHECK_EQ(json, R"({"a":1,"c":3,"b":2})");

    tr["a"] = 1;
    tr["b"] = 3;
    tr["c"] = 2;
    json = nos::json::to_string(tr);
    CHECK_EQ(json, R"({"a":1,"c":2,"b":3})");
}

TEST_CASE("trent_get")
{
    nos::trent tr = 3;
    CHECK_EQ(tr.get<int>(), 3);
    CHECK_EQ(std::stod(tr.get<std::string>()), 3);
}

TEST_CASE("const tremt")
{
    nos::trent tr = 3;
    const nos::trent &tr2 = tr;
    CHECK_EQ(tr2.get<int>(), 3);
    CHECK_EQ(std::stod(tr2.get<std::string>()), 3);
}

TEST_CASE("const trent dict")
{
    nos::trent tr;
    tr["a"] = 3;
    const nos::trent &tr2 = tr;
    CHECK_EQ(tr2["a"].as_numer_default(42), 3);
    const nos::trent &c = tr2["c"];
    CHECK(c.is_nil());
    CHECK_EQ(c.as_numer_default(42), 42);
    CHECK_EQ(&c, &nos::trent::static_nil());
}

TEST_CASE("nil")
{
    nos::trent tr;
    tr["a"] = nos::trent::nil();
    CHECK(tr["a"].is_nil());
    CHECK_EQ(tr["a"].as_numer_default(42), 42);

    tr["a"] = 3;
    CHECK_EQ(tr["a"].as_numer_default(42), 3);
}

TEST_CASE("parse nil")
{
    nos::trent tr = nos::json::parse("nil");
    CHECK(tr.is_nil());

    // nil in dict
    tr = nos::json::parse("{'a': nil}");
    CHECK(tr["a"].is_nil());
}

TEST_CASE("parse null")
{
    nos::trent tr = nos::json::parse("null");
    CHECK(tr.is_nil());

    // nil in dict
    tr = nos::json::parse("{'a': null}");
    CHECK(tr["a"].is_nil());
}