#include <cmath>
#include <doctest/doctest.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>

TEST_CASE("json_test.to_string")
{
    nos::trent tr;
    tr["a"].as_list();
    tr["b"].as_list();

    for (int i = 0; i < 3; i++)
    {
        tr["a"].as_list().push_back(i);
        tr["b"].as_list().push_back(i);
    }

    auto str = nos::json::to_string(tr);

    CHECK_EQ(str, R"({"a":[0,1,2],"b":[0,1,2]})");
}

TEST_CASE("json.parse")
{
    auto tr = nos::json::parse(R"( 
        {"cmd":"group_trajectory",
         "answer":false,
         "points":[
            [1,0],
            [0.939693,0.34202],
            [0.766044,0.642788],
            [0.5,0.866025],
            [0.173648,0.984808],
            [-0.173648,0.984808],
            [-0.5,0.866025],
            [-0.766044,0.642788],
            [-0.939693,0.34202],
            [-1,1.22461e-16],
            [-0.939693,-0.34202],
            [-0.766044,-0.642788],
            [-0.5,-0.866025],
            [-0.173648,-0.984808],
            [0.173648,-0.984808],
            [0.5,-0.866025],
            [0.766044,-0.642788],
            [0.939693,-0.34202],
            [1,-2.44921e-16]]}}
    )");

    CHECK(!tr.is_nil());

    CHECK_EQ(tr["points"][0][0].as_numer(), 1);
    CHECK_EQ(tr["points"][18][0].as_numer(), 1);
}
